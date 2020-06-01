#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

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
    void handleSendCallDrop(common::PhoneNumber) override;
    void handlePeerNotConnected(common::PhoneNumber) override;
    void handleUnknownRecipientAfterCallAccepted() override;


    //IUserEventsHandler interface
    void handleSendSms(common::PhoneNumber recipent, std::string message) override;
    void handleSendCallAccept(common::PhoneNumber recipient) override;
    void handleSendCallDropped(common::PhoneNumber recipient) override;
    void handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleSendCallRequest(common::PhoneNumber) override;
    void handleClose() override;
protected:
    Context& context;

private:
    common::PrefixedLogger logger;
};

}
