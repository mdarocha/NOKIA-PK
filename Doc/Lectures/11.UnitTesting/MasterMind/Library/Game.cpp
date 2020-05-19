#include "Game.hpp"

namespace MasterMind
{

Game::Game(IMatcher &matcher, Pattern pattern)
    : matcher(matcher),
      pattern(pattern)
{

}

void Game::run(Presentation &presentation)
{
    for (;;)
    {
        auto guess = presentation.getNextGuess(pattern.value.length());
        auto result = matcher.match(pattern, guess);
        presentation.printResult(result);
        if (result.exactMatches == pattern.value.length())
        {
             presentation.printSuccess();
             break;
        }
    }
}


}
