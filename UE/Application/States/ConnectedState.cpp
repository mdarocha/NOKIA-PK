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

void ConnectedState::handleReceivedSms(common::PhoneNumber sender, std::string message)
{
    context.db.saveReceivedSms(sender, message);
    context.user.showNewSms();
}

void ConnectedState::handleReceivedCallRequest(common::PhoneNumber recipient)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(30000ms);
    context.user.showCallRequest(recipient);
}

void ConnectedState::handleSendCallAccept(common::PhoneNumber recipient)
{
    context.bts.sendCallAccept(recipient);
    context.setState<TalkingState>(recipient);
}

void ConnectedState::handleSendCallDropped(common::PhoneNumber recipient)
{
    context.user.showConnected();
    context.bts.sendCallDropped(recipient);
}

void ConnectedState::handleTimeout()
{
   context.logger.logDebug("hanldeTimeoutConnectedState");
//   context.user.showConnected();
//   context.bts.sendCallDropped();
//   handleSendCallDropped()
}

}
