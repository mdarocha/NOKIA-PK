#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/BtsPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/IncomingMessage.hpp"

namespace ue
{
using namespace ::testing;

class BtsPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{13121981ll};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsEventsHandlerMock> handlerMock;
    StrictMock<common::ITransportMock> transportMock;
    common::ITransport::MessageCallback messageCallback;
    common::ITransport::DisconnectedCallback disconnectCallback;

    BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

    BtsPortTestSuite()
    {
        EXPECT_CALL(transportMock, registerMessageCallback(_))
                .WillOnce(SaveArg<0>(&messageCallback));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(_))
                .WillOnce(SaveArg<0>(&disconnectCallback));
        objectUnderTest.start(handlerMock);
    }
    ~BtsPortTestSuite()
    {
        EXPECT_CALL(transportMock, registerMessageCallback(IsNull()));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop)
{
}

TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage)
{
    common::OutgoingMessage wrongMsg{};
    wrongMsg.writeBtsId(BTS_ID);
    messageCallback(wrongMsg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleSib)
{
    EXPECT_CALL(handlerMock, handleSib(BTS_ID));
    common::OutgoingMessage msg{common::MessageId::Sib,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeBtsId(BTS_ID);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachAccept)
{
    EXPECT_CALL(handlerMock, handleAttachAccept());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(true);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachReject)
{
    EXPECT_CALL(handlerMock, handleAttachReject());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendAttachRequest)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    objectUnderTest.sendAttachRequest(BTS_ID);
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()) );
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendSms)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    auto recipent = common::PhoneNumber{123};
    auto message = "labjkdshfkjsdfhkjshfkshfj";

    objectUnderTest.sendSms(recipent, message);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::Sms, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipent, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(message, reader.readRemainingText()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedSms)
{
    EXPECT_CALL(handlerMock, handleReceivedSms(common::PhoneNumber{123}, "test"));

    common::OutgoingMessage msg{common::MessageId::Sms,
                                common::PhoneNumber{123},
                                PHONE_NUMBER};
    msg.writeText("test");
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedCallRequest)
{
    EXPECT_CALL(handlerMock, handleReceivedCallRequest(common::PhoneNumber{123}));
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                common::PhoneNumber{123},
                                PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedCallAccepted)
{
    EXPECT_CALL(handlerMock, handleReceivedCallAccepted(common::PhoneNumber{123}));
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                               common::PhoneNumber{123},
                               PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedCallDropped)
{
    EXPECT_CALL(handlerMock, handleReceivedCallDropped(common::PhoneNumber{123}));
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                common::PhoneNumber{123},
                                PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallAccept)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    auto recipent = common::PhoneNumber{123};

    objectUnderTest.sendCallAccept(recipent);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallAccepted, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipent, reader.readPhoneNumber()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallRequest)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    auto recipent = common::PhoneNumber{123};

    objectUnderTest.sendCallRequest(recipent);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallRequest, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipent, reader.readPhoneNumber()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallDropped)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    auto recipent = common::PhoneNumber{123};

    objectUnderTest.sendCallDropped(recipent);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipent, reader.readPhoneNumber()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleDisconnect)
{
    EXPECT_CALL(handlerMock, handleDisconnect());
    disconnectCallback();
}

TEST_F(BtsPortTestSuite, shallHandleUnknownRecipientToCallRequest)
{
    auto failTo = common::PhoneNumber{124};

    EXPECT_CALL(handlerMock, handlePeerNotConnected(failTo));

    common::OutgoingMessage msg{common::MessageId::UnknownRecipient,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeMessageId(common::MessageId::CallRequest);
    msg.writePhoneNumber(PHONE_NUMBER);
    msg.writePhoneNumber(failTo);

    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleUnknownRecipientToCallAccept)
{
    EXPECT_CALL(handlerMock, handleUnknownRecipientAfterCallAccepted());

    common::OutgoingMessage msg{common::MessageId::UnknownRecipient,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeMessageId(common::MessageId::CallAccepted);
    msg.writePhoneNumber(PHONE_NUMBER);
    msg.writePhoneNumber(common::PhoneNumber{});

    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallTalk)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    common::PhoneNumber recipient{123};
    std::string text = "test";

    objectUnderTest.sendCallTalk(recipient, text);

    common::IncomingMessage reader(msg);

    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallTalk, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipient, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(text, reader.readRemainingText()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedCallTalk)
{
    EXPECT_CALL(handlerMock, handleReceivedCallTalk(common::PhoneNumber{123}, "test"));
    common::OutgoingMessage msg{common::MessageId::CallTalk,
                               common::PhoneNumber{123},
                               PHONE_NUMBER};
    msg.writeText("test");
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleUnknownRecipientToCallTalk)
{
    common::PhoneNumber recipient{123};
    EXPECT_CALL(handlerMock, handlePeerNotConnected(recipient));

    common::OutgoingMessage msg{common::MessageId::UnknownRecipient,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeMessageId(common::MessageId::CallTalk);
    msg.writePhoneNumber(PHONE_NUMBER);
    msg.writePhoneNumber(recipient);

    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallDroppedAfterCallAccept)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });

    auto recipent = common::PhoneNumber{123};

    objectUnderTest.sendCallDropped(PHONE_NUMBER,recipent);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(recipent, reader.readPhoneNumber()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

}
