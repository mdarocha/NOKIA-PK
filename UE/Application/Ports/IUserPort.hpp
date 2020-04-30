#pragma once
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleSendSms(common::PhoneNumber recipent, std::string message) = 0;
    virtual void handleSendCallAccept(common::PhoneNumber recipient) = 0;
    virtual void handleSendCallDropped(common::PhoneNumber recipient) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewSms() = 0;
    virtual void showCallRequest(common::PhoneNumber) = 0;
};

}
