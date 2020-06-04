#pragma once

#include<string>
#include "sqlite_orm.h"
#include "Messages/PhoneNumber.hpp"
#include "Messages/BtsId.hpp"
#include "Logger/PrefixedLogger.hpp"
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
        DbPort(common::PhoneNumber number, common::ILogger& logger);
        void start();
        void stop();

        int saveSentSms(const common::PhoneNumber& to, const std::string& message) override;
        int saveReceivedSms(const common::PhoneNumber& from, const std::string& message) override;
        std::vector<DbMessage> getAllMessages() override;
        std::unique_ptr<DbMessage> getMessage(int id) override;
        void markAsRead(int id) override;

        void storeBtsId(const common::BtsId id) override;
    private:
        common::PhoneNumber phoneNumber;
        common::PrefixedLogger logger;
        std::string databasePath;
        std::unique_ptr<Storage> db;

        common::BtsId btsIdStorage;
};

}
