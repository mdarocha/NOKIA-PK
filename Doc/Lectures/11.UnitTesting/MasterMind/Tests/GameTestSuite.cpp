#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "IMatcherMock.hpp"
#include "Game.hpp"


namespace MasterMind
{

using namespace ::testing;

struct GameTestSuite : public Test
{
    IMatcherMock matcherMock;
    Pattern pattern{"ABCD"};
    Matcher<const Pattern&> matchPattern
       =  Field(&Pattern::value, pattern.value);
    Game objectUnderTest{matcherMock, pattern};
};

TEST_F(GameTestSuite, shallReturnTrueOnExactMatch)
{
    Guess guess{"ABCD"};
    GuessResult success {4u, 0u};

    EXPECT_CALL(matcherMock, match(matchPattern, _))
            .WillOnce(Return(success));

    ASSERT_TRUE(objectUnderTest.verify(guess));
}


}


