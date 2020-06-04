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

    MOCK_METHOD(int, saveSentSms, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(int, saveReceivedSms, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(std::vector<DbMessage>, getAllMessages, (), (final));
    MOCK_METHOD(std::optional<DbMessage>, getMessage, (int id), (final));
    MOCK_METHOD(void, markAsRead, (int id), (final));
    MOCK_METHOD(void, storeBtsId, (const common::BtsId), (final));
};

}
