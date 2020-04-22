#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void startTimer(Duration duration, common::PhoneNumber) override;
    void stopTimer() override;
    common::PhoneNumber getRecipient() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    common::PhoneNumber recipient;
    bool clearTimer = false;

    void setTimeOut(Duration);
};

}
