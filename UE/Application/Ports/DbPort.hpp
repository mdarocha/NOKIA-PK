#pragma once

#include<string>
#include "hiberlite.h"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

class DbPort : public IDbPort
{
    public:
        DbPort(common::PhoneNumber number);
        void start();
        void stop();

    private:
        std::string databasePath;
        hiberlite::Database db;
};

}
