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

TEST_F(ApplicationNotConnectedTestSuite, shallSetNotConnectedStateAtDisconnectEvent)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleDisconnect();
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

TEST_F(ApplicationConnectingTestSuite, shallHandleDisconnect)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleDisconnect();
}

TEST_F(ApplicationConnectingTestSuite, shallStoreNewSib)
{
    EXPECT_CALL(dbPortMock, storeBtsId(BTS_ID));
    objectUnderTest.handleSib(BTS_ID);
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

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivedSms)
{
    auto sender = common::PhoneNumber{124};
    auto message = "witaj";

    EXPECT_CALL(userPortMock, showNewSms());
    EXPECT_CALL(dbPortMock, saveReceivedSms(sender, message));

    objectUnderTest.handleReceivedSms(sender, message);
}

TEST_F(ApplicationConnectedTestSuite, shallSendCallRequest)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(btsPortMock, sendCallRequest(recipent));
    EXPECT_CALL(timerPortMock, startTimer(_));

    objectUnderTest.handleSendCallRequest(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivedCallRequest)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(timerPortMock, startTimer(_));
    EXPECT_CALL(userPortMock, showCallRequest(recipent));

    objectUnderTest.handleReceivedCallRequest(recipent);
}


TEST_F(ApplicationConnectedTestSuite, shallSendCallDropped)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(btsPortMock, sendCallDropped(recipent));

    objectUnderTest.handleSendCallDropped(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivedCallAccept)
{
    using namespace std::chrono_literals;
    common::PhoneNumber recipent{123};

    EXPECT_CALL(timerPortMock, startTimer(ue::TalkingMessageTimeout));
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showPeerConnected(recipent));

    objectUnderTest.handleReceivedCallAccepted(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivedCallDropped)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showCallDropped(recipent));

    objectUnderTest.handleReceivedCallDropped(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallSendCallDrop)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(btsPortMock, sendCallDropped(recipent));

    objectUnderTest.handleSendCallDropped(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallHandlePeerNotConnected)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showPeerNotConnected(recipent));

    objectUnderTest.handlePeerNotConnected(recipent);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleTimeout)
{
    EXPECT_CALL(userPortMock, callTimeout());
    objectUnderTest.handleTimeout();
}

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite {
    ApplicationTalkingTestSuite() {
        using namespace std::chrono_literals;
        common::PhoneNumber recipent{123};
        EXPECT_CALL(btsPortMock, sendCallAccept(recipent));
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(timerPortMock, startTimer(ue::TalkingMessageTimeout));
        objectUnderTest.handleSendCallAccept(recipent);
    }
};

TEST_F(ApplicationTalkingTestSuite, shallSendCallAccept)
{
    //constructor
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipentAfterAccepted)
{
    EXPECT_CALL(userPortMock, showPeerUserDisconnected());
    objectUnderTest.handleUnknownRecipientAfterCallAccepted();
}


//test for every state
TEST_F(ApplicationNotConnectedTestSuite, shallHandleClose)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleClose();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleClose)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleClose();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleClose)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleClose();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleClose)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleClose();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSendCallTalk)
{
    common::PhoneNumber recipient{123};
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(ue::TalkingMessageTimeout));
    EXPECT_CALL(btsPortMock, sendCallTalk(recipient, "test"));

    objectUnderTest.handleSendCallTalk("test");
}

TEST_F(ApplicationTalkingTestSuite, shallHandleReceivedCallTalk)
{
    common::PhoneNumber recipient{123};
    std::string text = "test";

    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(ue::TalkingMessageTimeout));
    EXPECT_CALL(userPortMock, showNewCallTalk(recipient, text));

    objectUnderTest.handleReceivedCallTalk(recipient, text);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleTimeout)
{
    common::PhoneNumber recipient{123};
    EXPECT_CALL(btsPortMock, sendCallDropped(recipient));
    EXPECT_CALL(userPortMock, showCallDropped(recipient));

    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(timerPortMock, stopTimer());

    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationTalkingTestSuite, shallHandlePeerNotConnectedAfterUnknowRecipient)
{
    common::PhoneNumber recipient{123};

    EXPECT_CALL(userPortMock, showPeerNotConnected(recipient));

    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(timerPortMock, stopTimer());

    objectUnderTest.handlePeerNotConnected(recipient);
}

}
