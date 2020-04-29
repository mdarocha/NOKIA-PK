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

}
