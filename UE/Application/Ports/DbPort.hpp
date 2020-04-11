#pragma once

#include<string>
#include "sqlite_orm.h"
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

inline auto initStorage(const std::string& path) 
{
    using namespace sqlite_orm;
    return make_storage(path,
            make_table("Messages",
                make_column("Id", &DbMessage::id,
                    primary_key(), autoincrement()),
                make_column("Text", &DbMessage::text),
                make_column("FromNumber", &DbMessage::fromNumber),
                make_column("ToNumber", &DbMessage::toNumber),
                make_column("Status", &DbMessage::status)
                )
            );
}

using Storage = decltype(initStorage(""));

class DbPort : public IDbPort
{
    public:
        DbPort(common::PhoneNumber number);
        void start();
        void stop();

        int saveSentSms(const common::PhoneNumber& to, const std::string& message) override;
        int saveReceivedSms(const common::PhoneNumber& from, const std::string& message) override;
        std::vector<DbMessage> getAllMessages() override;
    private:
        common::PhoneNumber phoneNumber;
        std::string databasePath;
        std::unique_ptr<Storage> db;
};

}
