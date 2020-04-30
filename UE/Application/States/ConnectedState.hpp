#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleSendSms(common::PhoneNumber recipent, std::string message) final;
    void handleReceivedSms(common::PhoneNumber sender, std::string message) final;
    void handleReceivedCallRequest(common::PhoneNumber recipient) final;
    void handleSendCallAccept(common::PhoneNumber recipient) final;
    void handleSendCallDropped(common::PhoneNumber recipient) final;
};

}
