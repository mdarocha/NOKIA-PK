#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
public:
    BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler& handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    void sendSms(common::PhoneNumber recipent, std::string message) override;
    void sendCallAccept(common::PhoneNumber recipient) override;
    void sendCallDropped(common::PhoneNumber recipient) override;
    void sendCallDropped(common::PhoneNumber from, common::PhoneNumber to) override;
    void sendCallRequest(common::PhoneNumber) override;
    void sendCallTalk(common::PhoneNumber, std::string) override;
private:
    void handleMessage(BinaryMessage msg);
    void handleDisconnected();

    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
