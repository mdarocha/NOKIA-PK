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

TEST_F(MatcherTestSuite, shallMatchExact)
{
    Pattern pattern{"abcd"};
    Guess guess{"abcd"};

    GuessResult result = objectUnderTest.match(pattern, guess);
    EXPECT_EQ(4u, result.exactMatches);
    EXPECT_EQ(0u, result.partialMatches);
}

}


