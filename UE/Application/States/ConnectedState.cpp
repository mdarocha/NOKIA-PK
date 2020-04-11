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
    context.db.saveSentSms(recipent, message);
    context.bts.sendSms(recipent, message);
}

void ConnectedState::handleReceivedSms(common::PhoneNumber sender, std::string message)
{
    context.db.saveReceivedSms(sender, message);
    context.user.showNewSms();
}

}
