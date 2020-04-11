#pragma once

#include<string>
#include "sqlite_orm.h"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

struct DbMessage {
    int id;
    std::string text;
    int fromNumber;
    int toNumber;
};

inline auto initStorage(const std::string& path) {
    using namespace sqlite_orm;
    return make_storage(path,
            make_table("Messages",
                make_column("Id", &DbMessage::id,
                    primary_key(), autoincrement()),
                make_column("Text", &DbMessage::text),
                make_column("FromNumber", &DbMessage::fromNumber),
                make_column("ToNumber", &DbMessage::toNumber)));
}
using Storage = decltype(initStorage(""));

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
        std::unique_ptr<Storage> db;
};

}
