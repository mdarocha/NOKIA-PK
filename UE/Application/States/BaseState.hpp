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
    void handleReceivedCallAccepted(common::PhoneNumber) override;
    void handleReceivedCallDropped(common::PhoneNumber) override;
    void handleDisconnect() override;
    void handleSendCallDrop(common::PhoneNumber) override;
    void handlePeerNotConnected(common::PhoneNumber) override;

    //IUserEventsHandler interface
    void handleSendSms(common::PhoneNumber recipent, std::string message) override;
    void handleSendCallRequest(common::PhoneNumber) override;
protected:
    Context& context;

private:
    common::PrefixedLogger logger;
};

}
