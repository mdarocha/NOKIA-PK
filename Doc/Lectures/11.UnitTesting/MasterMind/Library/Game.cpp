#include "Game.hpp"

namespace MasterMind
{

Game::Game(IMatcher &matcher, Pattern pattern)
    : matcher(matcher),
      pattern(pattern)
{

}

bool Game::verify(const Guess &guess)
{
    matcher.match(pattern, guess);
    return true;
}

}
