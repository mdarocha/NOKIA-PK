#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IDbPortMock.hpp"
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
    NiceMock<IDbPortMock> dbPortMock;

    Expectation notConnectedExpectation = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                dbPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {};

TEST_F(ApplicationNotConnectedTestSuite, shallSetNotConnectedStateAtStartup)
{

}

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite {
    ApplicationConnectingTestSuite() {
        using namespace std::chrono_literals;
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());
        objectUnderTest.handleSib(BTS_ID);
    }
};

TEST_F(ApplicationConnectingTestSuite, shallSendAttachRequestUponReceivingSIB)
{
    //constructor
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite {
    ApplicationConnectedTestSuite() {
        EXPECT_CALL(userPortMock, showConnected());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleAttachAccept();
    }
};

TEST_F(ApplicationConnectedTestSuite, shallShowConnectedOnAttachAccept)
{
    //constructor
}

TEST_F(ApplicationConnectedTestSuite, shallSendSms)
{
    auto recipent = common::PhoneNumber{123};
    auto message = "siema";

    EXPECT_CALL(btsPortMock, sendSms(recipent, message));
    EXPECT_CALL(dbPortMock, saveSentSms(recipent, message));

    objectUnderTest.handleSendSms(recipent, message);
}

TEST_F(ApplicationConnectedTestSuite, challHandleReceivedSms)
{
    auto sender = common::PhoneNumber{124};
    auto message = "witaj";
    
    EXPECT_CALL(userPortMock, showNewSms());
    EXPECT_CALL(dbPortMock, saveReceivedSms(sender, message));

    objectUnderTest.handleReceivedSms(sender, message);
}

}
