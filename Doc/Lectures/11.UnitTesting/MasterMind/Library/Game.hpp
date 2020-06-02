#pragma once

#include "IMatcher.hpp"
#include <iostream>

namespace MasterMind
{

struct Presentation
{
    std::istream& input;
    std::ostream& output;

    Guess getNextGuess(std::size_t length)
    {
        Guess guess;
        while (guess.value.length() != length)
        {
            output << "Next guess[" << length << "]: ";
            input >> guess.value;
        }
        return guess;
    }

    void printResult(const GuessResult& result)
    {
        output << "matches: " << std::string(result.exactMatches, '*') << std::string(result.partialMatches, 'o') << "\n";
    }

    void printSuccess()
    {
        output << "You guessed!\n";
    }

};


class Game
{
public:
    Game(IMatcher& matcher, Pattern pattern);

    void run(Presentation& presentation);

private:
    IMatcher& matcher;
    Pattern pattern;
};
}
