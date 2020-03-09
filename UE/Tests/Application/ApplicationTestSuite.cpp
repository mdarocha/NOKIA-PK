#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{203};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;

    Expectation notConnectedExpectation = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{};

TEST_F(ApplicationNotConnectedTestSuite, shallSetNotConnectedStateAtStartup)
{

}

TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestUponReceivingSIB)
{
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    objectUnderTest.handleSib(BTS_ID);
}
}
