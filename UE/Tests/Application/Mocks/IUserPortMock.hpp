#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleSendSms, (common::PhoneNumber, std::string), (final));
    MOCK_METHOD(void, handleSendCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleSendCallDrop, (common::PhoneNumber), (final));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showNewSms, (), (final));
    MOCK_METHOD(void, showNotAvailable, (common::PhoneNumber), (final));
    MOCK_METHOD(void, showPeerNotResponding, (common::PhoneNumber),(final));
};

}
