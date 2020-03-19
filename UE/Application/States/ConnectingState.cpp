#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

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
    context.logger.logDebug("attach accepted");
    context.setState<ConnectedState>();
}

void ue::ConnectingState::handleAttachReject()
{
    context.logger.logError("attach rejected");
    context.timer.stopTimer();
    context.setState<NotConnectedState>();
}

void ue::ConnectingState::handleTimeout()
{
    context.logger.logError("attach timeout");
    context.setState<NotConnectedState>();
}

}
