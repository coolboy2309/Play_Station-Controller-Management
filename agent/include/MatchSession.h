#pragma once

#include <string>
#include "GameState.h"

class MatchSession
{
public:

    std::string stationName;

    std::string startTime;

    int currentMinute;

    int currentSecond;

    MatchState state;

    bool running;

    double revenue;

    MatchSession();
};