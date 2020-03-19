#pragma once

#include "IMatcher.hpp"
namespace MasterMind
{


class Game
{
public:
    Game(IMatcher& matcher, Pattern pattern);
    bool verify(Guess const& guess);

private:
    IMatcher& matcher;
    Pattern pattern;
};
}
