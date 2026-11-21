
#pragma once

#include <string>
#include <deque>

class TimerStabilizer
{
public:
    std::string Update(const std::string& timer);

private:
    std::deque<std::string> history;

    static const int MAX_HISTORY = 3;
    int emptyReadings = 0;
    static const int MAX_EMPTY_READINGS = 2;
};
