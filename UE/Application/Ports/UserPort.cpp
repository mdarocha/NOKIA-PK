#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <sstream>
#include "UeGui/ICallMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber),
      currentView(CurrentView::Status),
      currentMode(nullptr)
{}

void UserPort::start(IUserEventsHandler &handler, IDbPort& dbPort)
{
    this->handler = &handler;
    this->dbPort = &dbPort;
    gui.setTitle("Nokia 3310: " + to_string(phoneNumber));
    gui.setAcceptCallback([this]() { handleAcceptClicked(); });
    gui.setRejectCallback([this]() { handleRejectClicked(); });
    gui.setCloseGuard([this]() { return closeGuard(); });
}

void UserPort::stop()
{
    handler = nullptr;
    dbPort = nullptr;
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::handleAcceptClicked()
{
    auto current = getCurrentMode();
    switch(current.first) {
        case CurrentView::HomeMenu: 
        {
            auto currentItem = ((IUeGui::IListViewMode*)current.second)->getCurrentItemIndex();
            if(currentItem.first && currentItem.second == UserPort::NewSmsItem) 
            {
                setCurrentMode(CurrentView::NewSms, &gui.setSmsComposeMode());
            }
            else if(currentItem.first && currentItem.second == UserPort::NewCallItem){
                setCurrentMode(CurrentView::NewCall, &gui.setDialMode());
            }
            else if(currentItem.first && currentItem.second == UserPort::ListSmsItem)
            {
                showSmsList();
            }
            break;
        }
        case CurrentView::NewSms:
        {
            auto menu = (IUeGui::ISmsComposeMode*)current.second;
            auto recipent = menu->getPhoneNumber();
            auto text = menu->getSmsText();
            handler->handleSendSms(recipent, text);

            menu->clearSmsText();
            showConnected();
            break;
        }
        case CurrentView::IncomingCall:
        {
            auto mode = (IUeGui::ICallMode *)currentMode;
            mode->appendIncomingText("Now you can talk");
            handler->handleSendCallAccept(getCurrentRecipent());
            setCurrentMode(CurrentView::Call, &gui.setCallMode());
            break;
        }
        case CurrentView::NewCall:
        {
            logger.logDebug("log debug: handleAcceptClicked()");
            auto mode = (IUeGui::IDialMode*)current.second;
            setCurrentRecipent(mode->getPhoneNumber());

            setCurrentMode(CurrentView::OutgoingCall, &gui.setCallMode());
            handler->handleSendCallRequest(getCurrentRecipent());

            auto newMode = (IUeGui::ICallMode*) currentMode;
            newMode->appendIncomingText("Calling to "+to_string(getCurrentRecipent()));
            break;
        }
        case CurrentView::Call:
        {
            auto menu = (IUeGui::ICallMode*) current.second;
            auto text = menu->getOutgoingText();
            menu->clearOutgoingText();
            menu->appendIncomingText("me: "+text);
            handler->handleSendCallTalk(text);
            break;
        }
        case CurrentView::SmsList:
        {
            auto currentItem = ((IUeGui::IListViewMode*)current.second)->getCurrentItemIndex();
            if(currentItem.first)
            {
                showSms(currentItem.second + 1);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void UserPort::handleRejectClicked()
{
    auto current = getCurrentMode();
    switch(current.first) {
        case CurrentView::NewSms: 
        {
            auto menu = (IUeGui::ISmsComposeMode*)current.second;
            menu->clearSmsText();
            showConnected();
            break;
        }
        case CurrentView::IncomingCall:
        {
            auto mode = (IUeGui::ICallMode *)currentMode;
            mode->appendIncomingText("Peer drop call");
            handler->handleSendCallDropped(getCurrentRecipent());
            break;
        }
        case CurrentView::NewCall:
        {
            logger.logDebug("Back to main menu");
            showConnected();
            break;
        }
        case CurrentView::OutgoingCall:
        {
            logger.logDebug("Call resignation");
            handler->handleSendCallDrop(getCurrentRecipent());
            auto menu = (IUeGui::ICallMode*)current.second;
            menu->appendIncomingText("Call resignation "+to_string(getCurrentRecipent()));
            showConnected();
            break;
        }
        case CurrentView::SmsList:
        {
            showConnected();
            break;
        }
        case CurrentView::TextView:
        {
            showSmsList();
            break;
        }
        case CurrentView::Call:
        {
            logger.logDebug("Call dropped after call accept");
            handler->handleSendCallDropped(phoneNumber, getCurrentRecipent());
            break;
        }
        default: {
            break;
        }
    }
}

bool UserPort::closeGuard()
{
    if(currentView == CurrentView::IncomingCall) {
        handler->handleSendCallDropped(getCurrentRecipent());
    } else if(currentView == CurrentView::OutgoingCall) {
        handler->handleSendCallDrop(getCurrentRecipent());
    }

    handler->handleClose();
    return true;
}

void UserPort::handleHomeClicked()
{

}

void UserPort::showNotConnected()
{
    currentView = CurrentView::Status;
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    currentView = CurrentView::Status;
    gui.showConnecting();
}

void UserPort::showConnected()
{
    gui.showConnected();

    auto menu = (IUeGui::IListViewMode*) &gui.setListViewMode();
    menu->clearSelectionList();
    menu->addSelectionListItem("Compose SMS", "Create new SMS");
    menu->addSelectionListItem("View SMS", "List all your messages");
    menu->addSelectionListItem("Make call", "Make new call");

    setCurrentMode(CurrentView::HomeMenu, menu);
}

void UserPort::showNewSms()
{
    gui.showNewSms();
}

void UserPort::showCallRequest(common::PhoneNumber recipient)
{
    if(currentView == CurrentView::IncomingCall || currentView == CurrentView::OutgoingCall)
    {
        handler->handleSendCallDrop(recipient);
    }
    else
    {
        setCurrentRecipent(recipient);
        setCurrentMode(CurrentView::IncomingCall, &gui.setCallMode());
        auto info = (IUeGui::ICallMode*) currentMode;
        info->appendIncomingText("Incoming call from " + to_string(recipient));
    }
}

void UserPort::showPeerUserDisconnected()
{
    auto info = (IUeGui::ICallMode*) currentMode;
    info->appendIncomingText("Unknown recipient");
}

void UserPort::showNotAvailable(common::PhoneNumber recipient)
{
    gui.showPeerUserNotAvailable(recipient);
}

void UserPort::showPeerNotConnected(common::PhoneNumber recipient)
{
    auto mode = (IUeGui::ICallMode *)currentMode;
    mode->appendIncomingText("Peer "+to_string(recipient)+" is not available");
    showConnected();
}

void UserPort::showPeerConnected(common::PhoneNumber recipient)
{
    auto mode = (IUeGui::ICallMode *)currentMode;
    mode->appendIncomingText("Connected to "+to_string(recipient));
    mode->appendIncomingText("Now you can talk");
    setCurrentMode(CurrentView::Call, mode);
}

void UserPort::showCallDropped(common::PhoneNumber recipient)
{
    auto mode = (IUeGui::ICallMode *)currentMode;
    mode->appendIncomingText("Peer "+to_string(recipient)+" drop call");
    showConnected();
}

void UserPort::showNewCallTalk(common::PhoneNumber recipient, std::string text)
{
    auto mode = (IUeGui::ICallMode*)currentMode;
    mode->appendIncomingText(to_string(recipient)+": "+text);
}

void UserPort::callTimeout()
{
    handler->handleSendCallDrop(recipientPhoneNumber);
    showConnected();
}

void UserPort::setCurrentRecipent(common::PhoneNumber recipient)
{
    recipientPhoneNumber = recipient;
}

common::PhoneNumber UserPort::getCurrentRecipent()
{
    return recipientPhoneNumber;
}

void UserPort::showSmsList()
{
    auto messages = dbPort->getAllMessages();
    auto menu = (IUeGui::IListViewMode*) &gui.setListViewMode();
    menu->clearSelectionList();

    if(messages.empty())
    {
        menu->addSelectionListItem("No messages to view :)", "No messages");
    }
    else
    {
        bool can_clear_icon = true;
        for(auto& m : messages)
        {
            if(m.fromNumber == phoneNumber.value)
            {
                menu->addSelectionListItem("To: " + std::to_string(m.toNumber), m.text);
            }
            else
            {
                if(m.status == (int)MessageStatus::not_read) {
                    menu->addSelectionListItem("! From: " + std::to_string(m.fromNumber), m.text);
                    can_clear_icon = false;
                } else {
                    menu->addSelectionListItem("From: " + std::to_string(m.fromNumber), m.text);
                }

            }
        }

        if(can_clear_icon) {
            gui.hideNewSms();
        }
    }
    setCurrentMode(CurrentView::SmsList, menu);
}

void UserPort::showSms(int id)
{
    auto menu = (IUeGui::ITextMode*) &gui.setViewTextMode();
    DbMessage message = dbPort->getMessage(id);

    std::ostringstream messageString;

    if(message.fromNumber == phoneNumber.value)
    {
        messageString << "To: " << message.toNumber;
    }
    else
    {
        messageString << "From: " << message.fromNumber;
    }
    messageString << std::endl << "---------" << std::endl;
    messageString << message.text << std::endl;
    menu->setText(messageString.str());
    setCurrentMode(CurrentView::TextView, menu);

    dbPort->markAsRead(id);
}

}
