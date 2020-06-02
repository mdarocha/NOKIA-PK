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
    Pattern pattern{"abcd"};
    Matcher<const Pattern&> matchPattern
       =  Field(&Pattern::value, pattern.value);
    Game objectUnderTest{matcherMock, pattern};
};

TEST_F(GameTestSuite, shallReturnOnFirstExactMatch)
{
    std::stringstream input{"abcd"};
    std::stringstream output;
    Presentation presentation{input, output};

    auto matchGuess = Field(&Guess::value, "abcd");
    GuessResult success {4u, 0u};

    EXPECT_CALL(matcherMock, match(matchPattern, matchGuess))
            .WillOnce(Return(success));

    objectUnderTest.run(presentation);
}


}


