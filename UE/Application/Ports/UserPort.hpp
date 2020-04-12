#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

enum class CurrentView {
    Status,
    HomeMenu,
    NewSms,
    SmsList,
    NewCall
};

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewSms() override;

    constexpr static unsigned NewSmsItem = 0;
    constexpr static unsigned ListSmsItem = 1;
    constexpr static unsigned NewCallItem = 2;

    std::pair<CurrentView, IUeGui::BaseMode*> getCurrentMode() { return std::pair(currentView, currentMode); };
    void setCurrentMode(CurrentView curView, IUeGui::BaseMode* mode) { currentView = curView; currentMode = mode; };

private:
    void handleAcceptClicked();
    void handleRejectClicked();
    void handleHomeClicked();

    CurrentView currentView;
    IUeGui::BaseMode* currentMode;

    common::PrefixedLogger logger;
    IUeGui& gui;

    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
