#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Mocks/IDbPortMock.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ISmsComposeModeMock> smsComposeModeMock;
    StrictMock<IDialModeMock> dialModeMock;
    StrictMock<ICallModeMock> callModeMock;
    StrictMock<IDbPortMock> dbPortMock;
    StrictMock<ITextModeMock> textModeMock;

    IUeGui::Callback acceptCallback;
    IUeGui::Callback rejectCallback;
    IUeGui::CloseGuard closeGuard;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        EXPECT_CALL(guiMock, setAcceptCallback(_))
            .WillOnce(SaveArg<0>(&acceptCallback));
        EXPECT_CALL(guiMock, setRejectCallback(_))
            .WillOnce(SaveArg<0>(&rejectCallback));
        EXPECT_CALL(guiMock, setCloseGuard(_))
            .WillOnce(SaveArg<0>(&closeGuard));
        objectUnderTest.start(handlerMock, dbPortMock);
    }
    ~UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setAcceptCallback(IsNull()));
        EXPECT_CALL(guiMock, setRejectCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMenuOnConnected)
{
    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.showConnected();
    auto currentMode = objectUnderTest.getCurrentMode();

    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowSmsComposeOnItemClick)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, UserPort::NewSmsItem)));
    EXPECT_CALL(guiMock, setSmsComposeMode()).WillOnce(ReturnRef(smsComposeModeMock));

    objectUnderTest.setCurrentMode(CurrentView::HomeMenu, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::NewSms);
    EXPECT_EQ(currentMode.second, &smsComposeModeMock);
}

TEST_F(UserPortTestSuite, shallShowSmsListOnItemClick)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, UserPort::ListSmsItem)));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));

    EXPECT_CALL(dbPortMock, getAllMessages());

    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::HomeMenu, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::SmsList);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowSmsDetail)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, 0)));
    EXPECT_CALL(guiMock, setViewTextMode()).WillOnce(ReturnRef(textModeMock));

    DbMessage msg{1, "test", 2, 3, 0};

    EXPECT_CALL(dbPortMock, getMessage(1)).WillOnce(Return(msg));
    EXPECT_CALL(dbPortMock, markAsRead(1));

    EXPECT_CALL(textModeMock, setText(_));

    objectUnderTest.setCurrentMode(CurrentView::SmsList, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::TextView);
    EXPECT_EQ(currentMode.second, &textModeMock);
}

TEST_F(UserPortTestSuite, shallHideNewMessageIconWhenAllMessagesRead)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, UserPort::ListSmsItem)));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));

    EXPECT_CALL(guiMock, hideNewSms());

    std::vector<DbMessage> msgs;
    msgs.push_back(DbMessage{ 0, "test", 123, 124, (int) MessageStatus::read });
    msgs.push_back(DbMessage{ 1, "test2", 123, 124, (int) MessageStatus::read });
    msgs.push_back(DbMessage{ 1, "test3", 124, 125, (int) MessageStatus::sent });
    EXPECT_CALL(dbPortMock, getAllMessages()).WillOnce(Return(msgs));

    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::HomeMenu, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::SmsList);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallNotHideNewMessageIconWhenNotAllMessagesRead)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, UserPort::ListSmsItem)));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));

    std::vector<DbMessage> msgs;
    msgs.push_back(DbMessage{ 0, "test", 123, 124, (int) MessageStatus::read });
    msgs.push_back(DbMessage{ 1, "test2", 123, 124, (int) MessageStatus::not_read });
    msgs.push_back(DbMessage{ 1, "test3", 124, 125, (int) MessageStatus::sent });
    EXPECT_CALL(dbPortMock, getAllMessages()).WillOnce(Return(msgs));

    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::HomeMenu, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::SmsList);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowDialOnItemClick)
{
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true, UserPort::NewCallItem)));
    EXPECT_CALL(guiMock, setDialMode()).WillOnce(ReturnRef(dialModeMock));

    objectUnderTest.setCurrentMode(CurrentView::HomeMenu, &listViewModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::NewCall);
    EXPECT_EQ(currentMode.second, &dialModeMock);
}

TEST_F(UserPortTestSuite, shallSendSmsEventOnSmsSend)
{
    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    const auto number = common::PhoneNumber{123};
    const std::string text = "siemka";

    EXPECT_CALL(smsComposeModeMock, getPhoneNumber()).WillOnce(Return(number));
    EXPECT_CALL(smsComposeModeMock, getSmsText()).WillOnce(Return(text));
    EXPECT_CALL(smsComposeModeMock, clearSmsText());

    EXPECT_CALL(handlerMock, handleSendSms(number, text));

    objectUnderTest.setCurrentMode(CurrentView::NewSms, &smsComposeModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallExitSmsCreationOnReject)
{
    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    EXPECT_CALL(smsComposeModeMock, clearSmsText());

    objectUnderTest.setCurrentMode(CurrentView::NewSms, &smsComposeModeMock);
    rejectCallback();
}

TEST_F(UserPortTestSuite, shallExitSmsList)
{
    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::SmsList, &listViewModeMock);
    rejectCallback();

    auto currentMode = objectUnderTest.getCurrentMode();

    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallExitSmsDetailView)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(dbPortMock, getAllMessages());
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::TextView, &textModeMock);
    rejectCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::SmsList);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowSMSReceivedIconOnSMSReceive)
{
    EXPECT_CALL(guiMock, showNewSms());
    objectUnderTest.showNewSms();
}

TEST_F(UserPortTestSuite, shallAcceptIncomingCall)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(handlerMock, handleSendCallAccept(recipent));
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));
    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));

    objectUnderTest.setCurrentRecipent(recipent);
    objectUnderTest.setCurrentMode(CurrentView::IncomingCall, &callModeMock);
    acceptCallback();
}

TEST_F(UserPortTestSuite, shallMakeCallRequest)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(dialModeMock, getPhoneNumber()).WillOnce(Return(recipent));
    EXPECT_CALL(handlerMock, handleSendCallRequest(recipent));
    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::NewCall, &dialModeMock);
    acceptCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    auto currentRecipent = objectUnderTest.getCurrentRecipent();

    EXPECT_EQ(currentRecipent, recipent);
    EXPECT_EQ(currentMode.first, CurrentView::OutgoingCall);
    EXPECT_EQ(currentMode.second, &callModeMock);
}

TEST_F(UserPortTestSuite, shallRejectIncomingCall)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(handlerMock, handleSendCallDropped(recipent));
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));

    objectUnderTest.setCurrentRecipent(recipent);
    objectUnderTest.setCurrentMode(CurrentView::IncomingCall, &callModeMock);
    rejectCallback();
}

TEST_F(UserPortTestSuite, shallReturnToMenuFromCall)
{
    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::NewCall, &dialModeMock);
    rejectCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallRejectOutgoingCall)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(handlerMock, handleSendCallDrop(recipent));
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));

    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentRecipent(recipent);
    objectUnderTest.setCurrentMode(CurrentView::OutgoingCall, &callModeMock);
    rejectCallback();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallHandleCallTimeout)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(handlerMock, handleSendCallDrop(recipent));

    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentRecipent(recipent);

    objectUnderTest.callTimeout();

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowCallRequest)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));

    objectUnderTest.showCallRequest(recipent);

    auto currentMode = objectUnderTest.getCurrentMode();
    auto currentRecipent = objectUnderTest.getCurrentRecipent();
    EXPECT_EQ(currentMode.first, CurrentView::IncomingCall);
    EXPECT_EQ(currentMode.second, &callModeMock);
    EXPECT_EQ(currentRecipent, recipent);
}

TEST_F(UserPortTestSuite, shallShowPeerDisconnected)
{
    EXPECT_CALL(callModeMock, appendIncomingText(_));

    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);
    objectUnderTest.showPeerUserDisconnected();
}

TEST_F(UserPortTestSuite, shallShowNotAvaible)
{
    common::PhoneNumber recipent{123};
    EXPECT_CALL(guiMock, showPeerUserNotAvailable(recipent));
    objectUnderTest.showNotAvailable(recipent);
}

TEST_F(UserPortTestSuite, shallShowPeerNotConnected)
{
    common::PhoneNumber recipent{123};
    EXPECT_CALL(callModeMock, appendIncomingText(_));

    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);

    objectUnderTest.showPeerNotConnected(recipent);

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallShowPeerConnected)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));
    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);

    objectUnderTest.showPeerConnected(recipent);

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::Call);
    EXPECT_EQ(currentMode.second, &callModeMock);
}

TEST_F(UserPortTestSuite, shallShowCallDropped)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AtLeast(1));

    EXPECT_CALL(guiMock, showConnected());
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);

    objectUnderTest.showCallDropped(recipent);

    auto currentMode = objectUnderTest.getCurrentMode();
    EXPECT_EQ(currentMode.first, CurrentView::HomeMenu);
    EXPECT_EQ(currentMode.second, &listViewModeMock);
}

TEST_F(UserPortTestSuite, shallEmitCloseEvent)
{
    EXPECT_CALL(handlerMock, handleClose());
    bool shouldClose = closeGuard();
    EXPECT_EQ(shouldClose, true);
}

TEST_F(UserPortTestSuite, shallDropIncomingCallOnClose)
{
    objectUnderTest.setCurrentMode(CurrentView::IncomingCall, &callModeMock);

    EXPECT_CALL(handlerMock, handleSendCallDropped(_));
    EXPECT_CALL(handlerMock, handleClose());
    bool shouldClose = closeGuard();
    EXPECT_EQ(shouldClose, true);
}

TEST_F(UserPortTestSuite, shallDropOutgoingCallOnClose)
{
    objectUnderTest.setCurrentMode(CurrentView::OutgoingCall, &callModeMock);

    EXPECT_CALL(handlerMock, handleSendCallDrop(_));
    EXPECT_CALL(handlerMock, handleClose());
    bool shouldClose = closeGuard();
    EXPECT_EQ(shouldClose, true);
}

TEST_F(UserPortTestSuite, shallShowSendedCallTalk)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(callModeMock, getOutgoingText()).WillOnce(Return("witam"));
    EXPECT_CALL(callModeMock, clearOutgoingText());
    EXPECT_CALL(callModeMock, appendIncomingText("me: witam")).Times(AtLeast(1));
    EXPECT_CALL(handlerMock, handleSendCallTalk("witam"));

    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);
    acceptCallback();
}

TEST_F(UserPortTestSuite, shallShowReceivedCallTalk)
{
    common::PhoneNumber recipient{123};
    std::string text = "test";

    EXPECT_CALL(callModeMock, appendIncomingText("123: "+text));

    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);
    objectUnderTest.showNewCallTalk(recipient, text);
}

TEST_F(UserPortTestSuite, shallDropCallAfterCallAccept)
{
    common::PhoneNumber recipent{123};

    EXPECT_CALL(handlerMock, handleSendCallDropped(PHONE_NUMBER, recipent));

    objectUnderTest.setCurrentRecipent(recipent);
    objectUnderTest.setCurrentMode(CurrentView::Call, &callModeMock);
    rejectCallback();
}

}
