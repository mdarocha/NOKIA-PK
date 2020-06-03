#include "TimerPort.hpp"

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
}

void TimerPort::startTimer(Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");

    std::thread thr{[=](){
        std::this_thread::sleep_for(duration);

        std::thread::id tid = std::this_thread::get_id();
        auto it = clearQueue.begin();
        while(it != clearQueue.end())
        {

            if(it->first == tid)
            {
                if(!it->second)
                {
                    handler->handleTimeout();
                }
                clearQueue.erase(it++);
                return;
            }
            it++;
        }
    }};
    clearQueue.push_back( std::make_pair(thr.get_id(), false) );
    thr.detach();
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");

    for(auto it = clearQueue.begin(); it != clearQueue.end(); ++it)
    {
        if(!it->second)
        {
            it->second = true;
            return;
        }
    }
}

}
