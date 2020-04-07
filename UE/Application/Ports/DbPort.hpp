#pragma once

#include<string>
#include "hiberlite.h"
#include "IDbPort.hpp"

namespace ue
{

class DbPort : public IDbPort
{
    public:
        DbPort(std::string dbPath);
        void start();
        void stop();

    private:
        std::string databasePath;
        hiberlite::Database db;
};

}
