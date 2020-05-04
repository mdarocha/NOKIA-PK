#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"

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

    IUeGui::Callback acceptCallback;
    IUeGui::Callback rejectCallback;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        EXPECT_CALL(guiMock, setAcceptCallback(_))
            .WillOnce(SaveArg<0>(&acceptCallback));
        EXPECT_CALL(guiMock, setRejectCallback(_))
            .WillOnce(SaveArg<0>(&rejectCallback));

        objectUnderTest.start(handlerMock);
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
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    EXPECT_CALL(smsComposeModeMock, clearSmsText());

    objectUnderTest.setCurrentMode(CurrentView::NewSms, &smsComposeModeMock);
    rejectCallback();
}

TEST_F(UserPortTestSuite, shallShowSMSReceivedIconOnSMSReceive)
{
    EXPECT_CALL(guiMock, showNewSms());
    objectUnderTest.showNewSms();
}

}
