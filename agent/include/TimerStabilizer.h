#pragma once

#include <string>
#include <deque>

class TimerStabilizer
{
public:

    std::string Update(const std::string& timer);

private:

    std::deque<std::string> history;

    static const int MAX_HISTORY = 5;
};