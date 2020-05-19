#include "Matcher.hpp"
#include "Game.hpp"
#include "Pattern.hpp"


int main(int argc, char* argv[])
{
    using namespace MasterMind;
    Matcher matcher;
    Pattern pattern{argc > 1 ? argv[1] : "abcd"};
    Game game(matcher, pattern);
    Presentation presentation{std::cin, std::cout};
    game.run(presentation);
}
