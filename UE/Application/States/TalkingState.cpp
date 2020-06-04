#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber recipient)
    :BaseState(context, "Talking State"),
      recipient(recipient)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(ue::TalkingMessageTimeout);
}

void TalkingState::handleUnknownRecipientAfterCallAccepted() {
    context.user.showPeerUserDisconnected();
}

void TalkingState::handleSendCallTalk(std::string text)
{
    using namespace std::chrono_literals;
    context.timer.stopTimer();
    context.timer.startTimer(ue::TalkingMessageTimeout);
    context.bts.sendCallTalk(recipient, text);
}

void TalkingState::handleReceivedCallTalk(common::PhoneNumber recipient, std::string text)
{
    using namespace std::chrono_literals;
    context.timer.stopTimer();
    context.timer.startTimer(ue::TalkingMessageTimeout);
    if(recipient == this->recipient)
        context.user.showNewCallTalk(recipient, text);
    else
        context.logger.logError("Recipient from actual talk is different than received.");
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDropped(recipient);
    context.user.showCallDropped(recipient);
    context.setState<ConnectedState>();
}

void TalkingState::handlePeerNotConnected(common::PhoneNumber recipient)
{
    context.user.showPeerNotConnected(recipient);
    context.setState<ConnectedState>();
}

void TalkingState::handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to) {
    context.bts.sendCallDropped(from, to);
    context.setState<ConnectedState>();
}

void TalkingState::handleReceivedCallDropped(common::PhoneNumber recipient)
{
    context.logger.logDebug("Recived Call dropped from ", recipient);
    if(recipient == this->recipient)
    {
        context.setState<ConnectedState>();
        context.user.showCallDropped(recipient);
    }
}

void TalkingState::handleReceivedCallRequest(common::PhoneNumber recipient)
{
    context.bts.sendCallDropped(recipient);
}

void TalkingState::handleClose()
{
    context.bts.sendCallDropped(recipient);
    context.timer.stopTimer();
}

}
