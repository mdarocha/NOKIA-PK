#pragma once

#include "IUeGui.hpp"
#include "BaseMode.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUeGui::IDialMode : public IUeGui::BaseMode
{
public:
    virtual ~IDialMode() = default;
    virtual PhoneNumber getPhoneNumber() const = 0;
};

}
