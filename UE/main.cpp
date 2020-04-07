#include "Application.hpp"
#include "ApplicationEnvironmentFactory.hpp"
#include "Ports/BtsPort.hpp"
#include "Ports/UserPort.hpp"
#include "Ports/TimerPort.hpp"
#include "Ports/DbPort.hpp"

int main(int argc, char* argv[])
{
    using namespace ue;
    using namespace std::chrono_literals;

    auto appEnv = ue::createApplicationEnvironment(argc, argv);
    auto& logger = appEnv->getLogger();
    auto& tranport = appEnv->getTransportToBts();
    auto& gui = appEnv->getUeGui();
    auto phoneNumber = appEnv->getMyPhoneNumber();

    BtsPort bts(logger, tranport, phoneNumber);
    UserPort user(logger, gui, phoneNumber);
    TimerPort timer(logger);

    auto dbName = std::string("db-");
    dbName.push_back(phoneNumber.value);
    DbPort db(dbName);

    Application app(phoneNumber, logger, bts, user, timer, db);
    bts.start(app);
    user.start(app);
    timer.start(app);
    db.start();

    appEnv->startMessageLoop();

    bts.stop();
    user.stop();
    timer.stop();
    db.stop();
}

