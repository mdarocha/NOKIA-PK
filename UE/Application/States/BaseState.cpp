#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}


void BaseState::handleSendSms(common::PhoneNumber recipent, std::string message)
{
    logger.logError("Uexpected: handleSendSms");
}

void BaseState::handleSendCallRequest(common::PhoneNumber)
{
    logger.logError("Uexpected: handleSendCallRequest");
}
  
void BaseState::handleDisconnect()
{
    logger.logError("Uexpected: handleDisconnect");
}

void BaseState::handleReceivedSms(common::PhoneNumber sender, std::string message)
{
    logger.logError("Uexpected: handleReceivedSms");
}

void BaseState::handleReceivedCallRequest(common::PhoneNumber recipient)
{
    logger.logError("Uexpected: handleReceivedCallRequest");
}

void BaseState::handleSendCallAccept(common::PhoneNumber recipient)
{
    logger.logError("Uexpected: handleSendCallAccept");
}

void BaseState::handleSendCallDropped(common::PhoneNumber recipient)
{
    logger.logError("Uexpected: handleSendCallDroppped");
}

void BaseState::handleSendCallDropped(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logError("Uexpected: handleSendCallDropppedAfterCallAccept");
}

void BaseState::handleUnknownRecipientAfterCallAccepted()
{
    logger.logError("Uexpected: handleUnknownRecipientAfterCallAccepted");
}

void BaseState::handleSendCallDrop(common::PhoneNumber)
{
    logger.logError("Unexpected: handleSendCallDrop");
}

void BaseState::handleReceivedCallAccepted(common::PhoneNumber recipient)
{
    logger.logError("Unexpected: handleReceivedCallAccepted from", recipient);
}

void BaseState::handleReceivedCallDropped(common::PhoneNumber recipient)
{
    logger.logError("Unexpected: handleReceivedCallDropped from", recipient);
}

void BaseState::handlePeerNotConnected(common::PhoneNumber recipient)
{
    logger.logError("Unexpected: handleUnknownRecipient after CallRequest");
}

void BaseState::handleSendCallTalk(std::string)
{
    logger.logError("Unexpected: handleSendCallTalk");
}

void BaseState::handleReceivedCallTalk(common::PhoneNumber, std::string)
{
    logger.logError("Unexpected: handleReceivedCallTalk");
}

void BaseState::handleClose()
{
    context.timer.stopTimer();
}

}
