#include "NotConnectedState.hpp"
#include "ConnectingState.hpp"

namespace ue
{

NotConnectedState::NotConnectedState(Context &context)
    : BaseState(context, "NotConnectedState")
{
    context.user.showNotConnected();
    context.timer.stopTimer();
}

void NotConnectedState::handleSib(common::BtsId btsId)
{
    context.setState<ConnectingState>(btsId);
}

}
