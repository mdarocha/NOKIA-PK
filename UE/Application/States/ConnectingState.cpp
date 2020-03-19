#include "ConnectingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();

    using namespace std::chrono_literals;
    context.bts.sendAttachRequest(btsId);
    context.timer.startTimer(500ms);
}

void ue::ConnectingState::handleAttachAccept()
{
    context.setState<ConnectedState>();
}
}
