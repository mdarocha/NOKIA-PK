#include "DbPort.hpp"
#include <sstream>
#include "sqlite_orm.h"
using namespace sqlite_orm;

namespace ue
{

DbPort::DbPort(common::PhoneNumber number) : phoneNumber(number)
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
    DbMessage msg{ -1, message, phoneNumber.value, to.value };
    db->insert(msg);
}

int DbPort::saveReceivedSms(const common::PhoneNumber& from, const std::string& message)
{
    //TODO
}

}
