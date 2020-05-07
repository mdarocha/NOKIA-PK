#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
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

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
    gui.setAcceptCallback([this]() { handleAcceptClicked(); });
    gui.setRejectCallback([this]() { handleRejectClicked(); });
}

void UserPort::stop()
{
    handler = nullptr;
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::handleAcceptClicked()
{
    auto current = getCurrentMode();
    switch(current.first) {
        case CurrentView::HomeMenu: {
            auto currentItem = ((IUeGui::IListViewMode*)current.second)->getCurrentItemIndex();
            if(currentItem.first && currentItem.second == UserPort::NewSmsItem) {
                setCurrentMode(CurrentView::NewSms, &gui.setSmsComposeMode());
            }
            else if(currentItem.first && currentItem.second == UserPort::NewCallItem){
                setCurrentMode(CurrentView::NewCall, &gui.setDialMode());
            }
            break;
        }
        case CurrentView::NewSms: {
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
            handler->handleSendCallAccept(recipientPhoneNumber);
            setCurrentMode(CurrentView::Call, &gui.setCallMode());
            break;
        }
        case CurrentView::NewCall:{
            logger.logDebug("log debug: handleAcceptClicked()");
            auto mode = (IUeGui::IDialMode*)current.second;
            recipientPhoneNumber = mode->getPhoneNumber();

            setCurrentMode(CurrentView::OutgoingCall, &gui.setCallMode());
            handler->handleSendCallRequest(recipientPhoneNumber);

            auto newMode = (IUeGui::ICallMode*) currentMode;
            newMode->appendIncomingText("Calling to "+to_string(recipientPhoneNumber));
            break;
        }
        case CurrentView::Call:{
            //to be implemented
            break;
        }
        default: {
            break;
         }
    }
}

void UserPort::handleRejectClicked()
{
    auto current = getCurrentMode();
    switch(current.first) {
        case CurrentView::NewSms: {
            auto menu = (IUeGui::ISmsComposeMode*)current.second;
            menu->clearSmsText();
            showConnected();
            break;
        }
        case CurrentView::IncomingCall:
        {
            auto mode = (IUeGui::ICallMode *)currentMode;
            mode->appendIncomingText("Peer drop call");
            handler->handleSendCallDropped(recipientPhoneNumber);
            break;
        }
        case CurrentView::NewCall:{
            logger.logDebug("Back to main menu");
            showConnected();
            break;
        }
        case CurrentView::OutgoingCall:{
            logger.logDebug("Call resignation");
            handler->handleSendCallDrop(recipientPhoneNumber);
            auto menu = (IUeGui::ICallMode*)current.second;
            menu->appendIncomingText("Call resignation "+to_string(recipientPhoneNumber));
            showConnected();
            break;
        }
        default: {
            break;
        }
    }
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
    recipientPhoneNumber = recipient;
    setCurrentMode(CurrentView::IncomingCall, &gui.setCallMode());
    auto info = (IUeGui::ICallMode*) currentMode;
    info->appendIncomingText("Incomig from " + to_string(recipient));
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
    mode->appendIncomingText("Peer drop call");
    showConnected();
}

void UserPort::callTimeout()
{
    handler->handleSendCallDrop(recipientPhoneNumber);
    showConnected();
}

}
