#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         IDbPort &db)
    : context{iLogger, bts, user, timer, db},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleSendSms(common::PhoneNumber recipent, std::string message)
{
    context.state->handleSendSms(recipent, message);
}

void Application::handleSendCallRequest(common::PhoneNumber recipient)
{
    context.state->handleSendCallRequest(recipient);
}
  
void Application::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void Application::handleReceivedSms(common::PhoneNumber sender, std::string message)
{
    context.state->handleReceivedSms(sender, message);
}

void Application::handleReceivedCallRequest(common::PhoneNumber recipient)
{
    context.state->handleReceivedCallRequest(recipient);
}

void Application::handleSendCallAccept(common::PhoneNumber recipient)
{
    context.state->handleSendCallAccept(recipient);
}

void Application::handleSendCallDropped(common::PhoneNumber recipient)
{
    context.state->handleSendCallDropped(recipient);
}

void Application::handleUnknownRecipientAfterCallAccepted()
{
    context.state->handleUnknownRecipientAfterCallAccepted();
}

void Application::handleSendCallDrop(common::PhoneNumber recipient)
{
    context.state->handleSendCallDrop(recipient);
}

void Application::handleReceivedCallAccepted(common::PhoneNumber recpient)
{
    context.state->handleReceivedCallAccepted(recpient);
}

void Application::handleReceivedCallDropped(common::PhoneNumber recpient)
{
    context.state->handleReceivedCallDropped(recpient);
}

void Application::handlePeerNotConnected(common::PhoneNumber recipient)
{
    context.state->handlePeerNotConnected(recipient);

}

}
