#pragma once

#include "BaseState.hpp"

namespace ue
{
constexpr std::chrono::milliseconds TalkingMessageTimeout{120000};

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber recipient);
    void handleUnknownRecipientAfterCallAccepted() final;
    void handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) final;
    void handleReceivedCallDropped(common::PhoneNumber recipient) final;
    void handleSendCallTalk(std::string) final;
    void handleReceivedCallTalk(common::PhoneNumber, std::string) override;
    void handleTimeout() override;
    void handlePeerNotConnected(common::PhoneNumber) override;
    void handleReceivedCallRequest(common::PhoneNumber) override;
    void handleClose() override;
    void handleReceivedSms(common::PhoneNumber sender, std::string message) override;
private:
    common::PhoneNumber recipient;
};

}
