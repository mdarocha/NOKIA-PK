#include "TalkingState.hpp"


namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber recipient)
    :BaseState(context, "Talking State"),
      recipient(recipient)
{
}

void TalkingState::handleUnknownRecipientAfterCallAccepted(){
    context.user.showPeerUserDisconnected();
}

void TalkingState::handleSendCallTalk(std::string text)
{
    context.bts.sendCallTalk(recipient, text);
}

void TalkingState::handleReceivedCallTalk(common::PhoneNumber recipient, std::string text)
{
    if(recipient == this->recipient)
        context.user.showNewCallTalk(recipient, text);
    else
        context.logger.logError("Recipient from actual talk is different than received.");
}

}
