#pragma once

#include "Ports/IDbPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"
#include "States/TalkingState.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                IDbPort& db);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleReceivedSms(common::PhoneNumber sender, std::string message) override;
    void handleReceivedCallRequest(common::PhoneNumber recipient) override;
    void handleReceivedCallAccepted(common::PhoneNumber) override;
    void handleReceivedCallDropped(common::PhoneNumber) override;
    void handleDisconnect() override;
    void handleUnknownRecipientAfterCallAccepted() override;

    void handleSendCallDrop(common::PhoneNumber) override;
    virtual void handlePeerNotConnected(common::PhoneNumber) override;
    void handleReceivedCallTalk(common::PhoneNumber, std::string) override;

    //IUserEventsHandler interface
    void handleSendSms(common::PhoneNumber recipent, std::string message) override;
    void handleSendCallAccept(common::PhoneNumber recipent) override;
    void handleSendCallDropped(common::PhoneNumber recipent) override;
    void handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleSendCallRequest(common::PhoneNumber recipient) override;
    void handleSendCallTalk(std::string) override;
    void handleClose() override;
private:
    Context context;
    common::PrefixedLogger logger;

};

}
