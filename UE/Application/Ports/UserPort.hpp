#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

enum class CurrentView {
    Status,
    HomeMenu,
    NewSms,
    SmsList
};

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler, IDbPort& dbPort);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewSms() override;
    void showSmsList() override;

    constexpr static unsigned NewSmsItem = 0;
    constexpr static unsigned ListSmsItem = 1;

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
    IDbPort* dbPort = nullptr;
};

}
