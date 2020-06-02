#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

enum class CurrentView {
    Status,
    HomeMenu,
    NewSms,
    SmsList,
    TextView,
    IncomingCall,
    OutgoingCall,
    NewCall,
    Call
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
    void showSms(int id) override;
    void showCallRequest(common::PhoneNumber) override;
    void showPeerUserDisconnected() override;
    void showNotAvailable(common::PhoneNumber) override;
    void showPeerNotConnected(common::PhoneNumber) override;
    void showPeerConnected(common::PhoneNumber) override;
    void showCallDropped(common::PhoneNumber) override;
    void showNewCallTalk(common::PhoneNumber, std::string) override;
    void callTimeout() override;

    constexpr static unsigned NewSmsItem = 0;
    constexpr static unsigned ListSmsItem = 1;
    constexpr static unsigned NewCallItem = 2;

    std::pair<CurrentView, IUeGui::BaseMode*> getCurrentMode() { return std::pair(currentView, currentMode); };
    void setCurrentMode(CurrentView curView, IUeGui::BaseMode* mode) { currentView = curView; currentMode = mode; };

    void setCurrentRecipent(common::PhoneNumber recipent);
    common::PhoneNumber getCurrentRecipent();
private:
    void handleAcceptClicked();
    void handleRejectClicked();
    void handleHomeClicked();
    bool closeGuard();

    CurrentView currentView;
    IUeGui::BaseMode* currentMode;

    common::PrefixedLogger logger;
    IUeGui& gui;

    common::PhoneNumber phoneNumber;
    common::PhoneNumber recipientPhoneNumber;
    IUserEventsHandler* handler = nullptr;
    IDbPort* dbPort = nullptr;
};

}
