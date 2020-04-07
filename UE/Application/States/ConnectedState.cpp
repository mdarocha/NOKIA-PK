#include "ConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.timer.stopTimer();
}

void ConnectedState::handleSendSms(common::PhoneNumber recipent, std::string message)
{
    context.bts.sendSms(recipent, message);
}

}
