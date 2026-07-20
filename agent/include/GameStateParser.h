#pragma once

#include <string>

class GameStateParser
{
public:

    static int GetMinute(const std::string& timer);

    static int GetSecond(const std::string& timer);
};