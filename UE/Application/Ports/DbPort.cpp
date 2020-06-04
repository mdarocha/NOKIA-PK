#include "DbPort.hpp"
#include <sstream>
#include "sqlite_orm.h"
using namespace sqlite_orm;

namespace ue
{

DbPort::DbPort(common::PhoneNumber number, common::ILogger& logger) : phoneNumber(number), logger(logger, "[DB-PORT]")
{
    std::ostringstream os;
    os << "db-" << (int) number.value << ".db";
    databasePath = os.str();

    db = std::make_unique<Storage>(initStorage(databasePath));
}

void DbPort::start()
{
    db->sync_schema(true);
}

void DbPort::stop()
{

}

int DbPort::saveSentSms(const common::PhoneNumber& to, const std::string& message)
{
    DbMessage msg{ -1, message, phoneNumber.value, to.value, (int) MessageStatus::sent };
    return db->insert(msg);
}

int DbPort::saveReceivedSms(const common::PhoneNumber& from, const std::string& message)
{
    DbMessage msg{ -1, message, from.value, phoneNumber.value, (int) MessageStatus::not_read };
    return db->insert(msg);
}

std::vector<DbMessage> DbPort::getAllMessages()
{
    return db->get_all<DbMessage>();
}

std::optional<DbMessage> DbPort::getMessage(int id)
{
    try{
        return db->get<DbMessage>(id);
    } catch (std::system_error &error) {
        return std::nullopt;
    }
}

void DbPort::markAsRead(int id)
{
    auto msg = db->get<DbMessage>(id);
    if(msg.status != (int)MessageStatus::sent) {
        msg.status = (int)MessageStatus::read;
        db->update<DbMessage>(msg);
    } else {

    }
}

void DbPort::storeBtsId(const common::BtsId id)
{
    //all according to the documentation :)
    if(id != btsIdStorage) {
        btsIdStorage = id;
    }
}

}
