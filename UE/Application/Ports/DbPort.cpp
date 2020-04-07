#include "DbPort.hpp"

namespace ue
{

DbPort::DbPort(std::string dbPath)
    : databasePath(dbPath)
{}

void DbPort::start()
{
    db.open(databasePath);
}

void DbPort::stop()
{

}

}

