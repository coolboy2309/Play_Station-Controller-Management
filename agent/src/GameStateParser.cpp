#include "../include/GameStateParser.h"

int GameStateParser::GetMinute(const std::string& timer)
{
    if (timer.length() < 5)
        return -1;

    return std::stoi(timer.substr(0, 2));
}

int GameStateParser::GetSecond(const std::string& timer)
{
    if (timer.length() < 5)
        return -1;

    return std::stoi(timer.substr(3, 2));
}