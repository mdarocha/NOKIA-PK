#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{

class IDbPort
{
public:
    virtual ~IDbPort() = default;
    virtual void saveSentSms(common::PhoneNumber to, std::string message) = 0;
};

}
