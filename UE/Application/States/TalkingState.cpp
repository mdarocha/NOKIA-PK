#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber recipient)
    :BaseState(context, "Talking State"),
      recipient(recipient)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(2min);
}

void TalkingState::handleUnknownRecipientAfterCallAccepted() {
    context.user.showPeerUserDisconnected();
}

void TalkingState::handleSendCallTalk(std::string text)
{
    using namespace std::chrono_literals;
    context.timer.stopTimer();
    context.timer.startTimer(2min);
    context.bts.sendCallTalk(recipient, text);
}

void TalkingState::handleReceivedCallTalk(common::PhoneNumber recipient, std::string text)
{
    using namespace std::chrono_literals;
    context.timer.stopTimer();
    context.timer.startTimer(2min);
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
    context.timer.stopTimer();
    context.user.showPeerNotConnected(recipient);
    context.setState<ConnectedState>();
}

}
