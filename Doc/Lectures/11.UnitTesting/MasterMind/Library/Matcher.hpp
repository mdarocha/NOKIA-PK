#pragma once

#include "GuessResult.hpp"
#include "Pattern.hpp"
#include "Guess.hpp"
#include "IMatcher.hpp"

namespace MasterMind
{

class Matcher : public IMatcher
{
public:
    GuessResult match(const Pattern& pattern, const Guess& guess);
};

}
