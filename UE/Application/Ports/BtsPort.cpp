#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this](){handleDisconnected();});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            auto message = reader.readRemainingText();
            logger.logDebug("receivedSms: ", from);
            handler->handleReceivedSms(from, message);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            handler->handleReciveCallAccepted(from);
            break;
        }
        case common::MessageId::CallDropped:
        {
            logger.logDebug("recived Call dropped");
            //to be implemented
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            logger.logDebug("recived Unknown Recipient");
            //to be implemented
            break;
        }
        default:
            logger.logError("unknow message: ", msgId, ", from: ", from);
        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::handleDisconnected()
{
    logger.logDebug("handleDesconnected");
    handler->handleDisconnect();
}

void BtsPort::sendSms(common::PhoneNumber recipent, std::string message)
{
    logger.logDebug("sendSms: ", recipent);
    common::OutgoingMessage msg{common::MessageId::Sms,
                                phoneNumber,
                                recipent};
    msg.writeText(message);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber recipient)
{
    logger.logDebug("sendCallReuest: ", recipient);
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                               phoneNumber,
                               recipient};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallDrop(common::PhoneNumber recipient)
{
    logger.logDebug("sendCallDrop: ", recipient);
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                               phoneNumber,
                               recipient};
}

}
