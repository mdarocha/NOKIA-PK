#include<thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/TimerPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using namespace ::testing;

class TimerPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<ITimerEventsHandlerMock> handlerMock;

    TimerPort objectUnderTest{loggerMock};

    TimerPortTestSuite()
    {
        objectUnderTest.start(handlerMock);
    }
    ~TimerPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(TimerPortTestSuite, shallHandleTimeout)
{
    EXPECT_CALL(handlerMock, handleTimeout());
    objectUnderTest.startTimer(std::chrono::duration<int>(1));
    std::this_thread::sleep_for(std::chrono::duration<int>(2));
}

}
