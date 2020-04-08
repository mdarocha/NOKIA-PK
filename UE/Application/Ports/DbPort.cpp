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
    db.open(databasePath);
    db.registerBeanClass<DbMessage>();

    db.createModel();
}

void DbPort::stop()
{

}

void DbPort::saveSentSms(common::PhoneNumber to, std::string message)
{
    auto msg = db.createBean<DbMessage>();
    msg->fromNumber = phoneNumber.value;
    msg->toNumber = to.value;
    msg->text = message;
}

void DbPort::saveReceivedSms(common::PhoneNumber from, std::string message)
{
    auto msg = db.createBean<DbMessage>();
    msg->fromNumber = from.value;
    msg->toNumber = phoneNumber.value;
    msg->text = message;
}

}

HIBERLITE_EXPORT_CLASS(ue::DbMessage)
