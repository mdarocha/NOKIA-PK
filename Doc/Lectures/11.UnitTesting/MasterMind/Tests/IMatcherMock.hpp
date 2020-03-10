#include "IMatcher.hpp"

#include <gmock/gmock.h>

namespace MasterMind
{

class IMatcherMock : public IMatcher
{
public:
    MOCK_METHOD(GuessResult, match, (const Pattern&, const Guess&), (final));
};

}
