#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber recipient)
    :BaseState(context, "Talking State"),
      recipient(recipient)
{
}

void TalkingState::handleUnknownRecipientAfterCallAccepted() {
    context.user.showPeerUserDisconnected();
}

void TalkingState::handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) {
    context.bts.sendCallDropped(from, to);
    context.timer.stopTimer();
    context.setState<ConnectedState>();
    context.user.showConnected();
}

void TalkingState::handleReceivedCallDropped(common::PhoneNumber recipient)
{
    context.logger.logDebug("Recived Call dropped from ", recipient);
    context.timer.stopTimer();
    context.setState<ConnectedState>();
    context.user.showCallDropped(recipient);
}

}
