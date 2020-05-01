#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context,common::PhoneNumber number)
    : BaseState(context, "TalkingState")
{
    recipientNumber = number;
//    context.user.showConnected();
//    context.timer.stopTimer();
}

void TalkingState::handleSendCallAccept(common::PhoneNumber recipient){
    context.bts.sendCallAccept(recipient);
}

void TalkingState::handleUnknownRecipientAfterCallAccepted(){
    context.user.showPeerUserDisconnected();
}

}
