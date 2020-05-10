#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <sstream>

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
            if(currentItem.first && currentItem.second == UserPort::ListSmsItem)
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
        case CurrentView::SmsList:
        {
            auto currentItem = ((IUeGui::IListViewMode*)current.second)->getCurrentItemIndex();
            if(currentItem.first)
            {
                showSms(currentItem.second + 1);
            }
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
        case CurrentView::NewSms: 
        {
            auto menu = (IUeGui::ISmsComposeMode*)current.second;
            menu->clearSmsText();
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
        default: 
        {
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

    setCurrentMode(CurrentView::HomeMenu, menu);
}

void UserPort::showNewSms()
{
    gui.showNewSms();
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
        for(auto& m : messages)
        {
            if(m.fromNumber == phoneNumber.value)
            {
                menu->addSelectionListItem("To: " + std::to_string(m.toNumber), m.text);
            }
            else
            {
                menu->addSelectionListItem("From: " + std::to_string(m.fromNumber), m.text);
            }
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
}

}
