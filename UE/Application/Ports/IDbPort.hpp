#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>
#include <vector>

namespace ue
{
enum class MessageStatus
{
    not_read,
    read,
    sent
};

struct DbMessage 
{
    int id;
    std::string text;
    int fromNumber;
    int toNumber;
    MessageStatus status;
};

class IDbPort
{
public:
    virtual ~IDbPort() = default;

    virtual int saveSentSms(const common::PhoneNumber& to, const std::string& message) = 0;
    virtual int saveReceivedSms(const common::PhoneNumber& from, const std::string& message) = 0;
    virtual std::vector<DbMessage> getAllMessages() = 0;
};

}
