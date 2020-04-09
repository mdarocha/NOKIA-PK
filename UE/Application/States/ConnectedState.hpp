#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleSendSms(common::PhoneNumber recipent, std::string message) final;
};

}
