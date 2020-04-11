#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{

struct DbMessage {
    int id;
    std::string text;
    int fromNumber;
    int toNumber;
};

class IDbPort
{
public:
    virtual ~IDbPort() = default;

    virtual int saveSentSms(const common::PhoneNumber& to, const std::string& message) = 0;
    virtual int saveReceivedSms(const common::PhoneNumber& from, const std::string& message) = 0;
};

}
