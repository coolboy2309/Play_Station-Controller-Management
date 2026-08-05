#include "../include/TimerStabilizer.h"
#include <map>

std::string TimerStabilizer::Update(const std::string& timer)
{
    if (timer.empty())
    {
        if (history.empty())
            return "";

        return history.back();
    }

    history.push_back(timer);

    if (history.size() > MAX_HISTORY)
    {
        history.pop_front();
    }

    std::map<std::string, int> counter;

    for (const auto& t : history)
    {
        counter[t]++;
    }

    std::string bestTimer = history.back();
    int bestCount = 0;

    for (const auto& item : counter)
    {
        if (item.second > bestCount)
        {
            bestCount = item.second;
            bestTimer = item.first;
        }
    }

    return bestTimer;
}