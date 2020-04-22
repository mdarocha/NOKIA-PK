#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleSendSms(common::PhoneNumber recipent, std::string message) final;
    void handleSendCallRequest(common::PhoneNumber recipient) final;
    void handleReceivedSms(common::PhoneNumber sender, std::string message) final;
    void handleSendCallDrop(common::PhoneNumber) final;
    void handleTimeout() final;
};

}
