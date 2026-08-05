#include "../include/TimerValidator.h"

bool TimerValidator::IsValid(const std::string& timer)
{
    if (timer.size() != 5)
        return false;

    if (timer[2] != ':')
        return false;

    if (!isdigit(timer[0]) ||
        !isdigit(timer[1]) ||
        !isdigit(timer[3]) ||
        !isdigit(timer[4]))
        return false;

    int minute =
        std::stoi(timer.substr(0,2));

    int second =
        std::stoi(timer.substr(3,2));

    if (minute < 0 || minute > 130)
        return false;

    if (second < 0 || second > 59)
        return false;

    return true;
}

bool TimerValidator::IsSequential(
    const std::string& previous,
    const std::string& current)
{
    if(previous.empty())
        return true;

    int pm =
        std::stoi(previous.substr(0,2));

    int ps =
        std::stoi(previous.substr(3,2));

    int cm =
        std::stoi(current.substr(0,2));

    int cs =
        std::stoi(current.substr(3,2));

    int previousTotal =
        pm * 60 + ps;

    int currentTotal =
        cm * 60 + cs;

    int diff =
        currentTotal - previousTotal;

    if(diff < 0)
        return false;

    if(diff > 5)
        return false;

    return true;
}