#include "Matcher.hpp"
#include "Game.hpp"
#include "Pattern.hpp"

int main(int argc, char* argv[])
{
    using namespace MasterMind;
    Matcher matcher;
    Pattern pattern{argc == 1 ? argv[1] : "ABCD"};
    Game game(matcher, pattern);
}
