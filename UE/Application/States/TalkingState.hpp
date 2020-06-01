#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber recipient);
    void handleUnknownRecipientAfterCallAccepted() final;
    void handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) final;
    void handleReceivedCallDropped(common::PhoneNumber recipient) final;
private:    
	common::PhoneNumber recipient;
};

}
