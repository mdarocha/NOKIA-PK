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

};
}
