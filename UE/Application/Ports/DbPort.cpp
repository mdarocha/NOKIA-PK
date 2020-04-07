#include "DbPort.hpp"
#include <sstream>

namespace ue
{

DbPort::DbPort(common::PhoneNumber number)
{
    std::ostringstream os;
    os << "db-" << (int) number.value << ".db";
    databasePath = os.str();
}

void DbPort::start()
{
    db.open(databasePath);
}

void DbPort::stop()
{

}

}

