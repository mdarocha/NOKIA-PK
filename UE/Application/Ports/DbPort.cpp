#include "DbPort.hpp"
#include <sstream>

namespace ue
{

DbPort::DbPort(common::PhoneNumber number) : phoneNumber(number)
{
    std::ostringstream os;
    os << "db-" << (int) number.value << ".db";
    databasePath = os.str();
}

void DbPort::start()
{

}

void DbPort::stop()
{

}

void DbPort::saveSentSms(common::PhoneNumber to, std::string message)
{
}

void DbPort::saveReceivedSms(common::PhoneNumber from, std::string message)
{
    auto msg = db.createBean<DbMessage>();
    msg->fromNumber = from.value;
    msg->toNumber = phoneNumber.value;
    msg->text = message;
}

}
