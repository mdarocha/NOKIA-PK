#include "Matcher.hpp"

namespace MasterMind
{

GuessResult Matcher::match(const Pattern &pattern, const Guess &guess)
{
    GuessResult l_result{};
    l_result.exactMatches = pattern.value.length();
    return l_result;
}

}
