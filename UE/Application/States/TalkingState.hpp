#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber number);
    void handleSendCallAccept(common::PhoneNumber recipient) final;
    common::PhoneNumber recipientNumber;
};

}
