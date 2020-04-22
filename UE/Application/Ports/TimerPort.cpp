#include "TimerPort.hpp"
#include <thread>

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
    clearTimer = true;
}

void TimerPort::startTimer(Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    setTimeOut(duration);
}

void TimerPort::startTimer(Duration duration, common::PhoneNumber recipient)
{
    this->recipient = recipient;
    logger.logDebug("Start timer: ", duration.count(), "ms with number ",this->recipient);
    setTimeOut(duration);
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");
    clearTimer = true;
}

common::PhoneNumber TimerPort::getRecipient()
{
    return recipient;
}

void TimerPort::setTimeOut(Duration duration)
{
    clearTimer = false;
    std::thread t{[=](){
            if(clearTimer)
                return;
            std::this_thread::sleep_for(duration);
            if(clearTimer)
                return;
            handler->handleTimeout();
    }};
    t.detach();
}

}
