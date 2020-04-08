#pragma once

#include <gmock/gmock.h>
#include "Ports/IDbPort.hpp"

namespace ue
{

class IDbPortMock : public IDbPort
{
public:
    IDbPortMock();
    ~IDbPortMock() override;

    MOCK_METHOD(void, saveSentSms, (common::PhoneNumber, std::string), (final));
    MOCK_METHOD(void, saveReceivedSms, (common::PhoneNumber, std::string), (final));
};

}
