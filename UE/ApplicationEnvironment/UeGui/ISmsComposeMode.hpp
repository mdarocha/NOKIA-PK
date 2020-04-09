#pragma once

#include "IUeGui.hpp"
#include "BaseMode.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUeGui::ISmsComposeMode : public IUeGui::BaseMode
{
public:
    virtual ~ISmsComposeMode() = default;

    virtual PhoneNumber getPhoneNumber() const = 0;
    virtual std::string getSmsText() const = 0;
    virtual void clearSmsText()= 0;
};

}
