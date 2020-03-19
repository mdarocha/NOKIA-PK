#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Matcher.hpp"

namespace MasterMind
{

using namespace ::testing;

struct MatcherTestSuite : public Test
{
    Matcher objectUnderTest;
};

TEST_F(MatcherTestSuite, shallMatchNone)
{
    Pattern pattern{"AAAA"};
    Guess guess{"BBBB"};

    GuessResult result = objectUnderTest.match(pattern, guess);
    EXPECT_EQ(0u, result.exactMatches);
    EXPECT_EQ(0u, result.partialMatches);
}

}


