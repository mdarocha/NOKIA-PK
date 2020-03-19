#pragma once

#include "GuessResult.hpp"
#include "Pattern.hpp"
#include "Guess.hpp"

namespace MasterMind
{

class IMatcher
{
public:
    virtual ~IMatcher() = default;
    virtual GuessResult match(const Pattern& pattern, const Guess& guess) = 0;
};

}
