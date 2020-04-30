#include "ConnectedState.hpp"
#include "TalkingState.hpp"

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
    context.db.saveSentSms(recipent, message);
    context.bts.sendSms(recipent, message);
}

void ConnectedState::handleSendCallRequest(common::PhoneNumber recipient)
{
    using namespace std::chrono_literals;
    context.bts.sendCallRequest(recipient);
    context.timer.startTimer(60s);
}

void ConnectedState::handleReceivedSms(common::PhoneNumber sender, std::string message)
{
    context.db.saveReceivedSms(sender, message);
    context.user.showNewSms();
}

void ConnectedState::handleTimeout()
{
    context.logger.logDebug("connected state: timeout for number: ");
    context.user.callTimeout();
}

void ConnectedState::handleSendCallDrop(common::PhoneNumber recipient)
{
    context.logger.logDebug("Send call drop to", recipient);
    context.timer.stopTimer();
    context.bts.sendCallDrop(recipient);
}

void ConnectedState::handleReceivedCallAccepted(common::PhoneNumber recipient)
{
    context.logger.logDebug("Recived Call Accepted from ", recipient);
    context.timer.stopTimer();
    context.user.showPeerConnected(recipient);
    context.setState<TalkingState>(recipient);
}


void ConnectedState::handleReceivedCallDropped(common::PhoneNumber recipient)
{
    context.logger.logDebug("Recived Call dropped from ", recipient);
    context.timer.stopTimer();
    context.user.showCallDropped(recipient);
}

void ConnectedState::handlePeerNotConnected(common::PhoneNumber recipient)
{
    context.logger.logDebug("Recieved Unknown Recipient after CallRequest");
    context.timer.stopTimer();
    context.user.showPeerNotConnected(recipient);
}

}
