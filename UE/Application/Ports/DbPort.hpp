#pragma once

#include<string>
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

class DbMessage {
    public:
        std::string text;
        int fromNumber;
        int toNumber;
};

class DbPort : public IDbPort
{
    public:
        DbPort(common::PhoneNumber number);
        void start();
        void stop();

        void saveSentSms(common::PhoneNumber to, std::string message) override;
        void saveReceivedSms(common::PhoneNumber from, std::string message) override;
    private:
        common::PhoneNumber phoneNumber;
        std::string databasePath;
};

}
