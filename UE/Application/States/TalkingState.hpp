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
    void handleSendCallTalk(std::string) final;
    void handleReceivedCallTalk(common::PhoneNumber, std::string) override;
    void handleTimeout() override;
    void handlePeerNotConnected(common::PhoneNumber) override;

private:    
	common::PhoneNumber recipient;
};

}
