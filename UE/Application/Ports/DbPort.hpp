#pragma once

#include<string>
#include "hiberlite.h"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

class DbMessage {
    friend class hiberlite::access;
    template<class Archive>
    void hibernate(Archive & ar)
    {
        ar & HIBERLITE_NVP(text);
        ar & HIBERLITE_NVP(fromNumber);
        ar & HIBERLITE_NVP(toNumber);
    }

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
        hiberlite::Database db;
};

}
