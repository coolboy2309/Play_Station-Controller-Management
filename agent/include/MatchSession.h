#pragma once

#include <string>
#include <chrono>

struct MatchSession
{
    std::string sessionID;

    std::string competition;

    std::string station;

    std::string startTimer;

    std::string endTimer;

    bool extraTime = false;

    std::chrono::steady_clock::time_point startTime;

    std::chrono::steady_clock::time_point endTime;
};