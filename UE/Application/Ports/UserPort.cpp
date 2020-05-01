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
        case CurrentView::Call:
        {
            handler->handleSendCallAccept(recipientPhoneNumber);
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
        case CurrentView::Call:
        {
            handler->handleSendCallDropped(recipientPhoneNumber);
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

    setCurrentMode(CurrentView::HomeMenu, menu);
}

void UserPort::showNewSms()
{
    gui.showNewSms();
}

void UserPort::showCallRequest(common::PhoneNumber recipient)
{
    recipientPhoneNumber = recipient;
    setCurrentMode(CurrentView::Call, &gui.setCallMode());
    auto info = (IUeGui::ICallMode*) currentMode;
    info->appendIncomingText("From " + to_string(recipient));
}

}
