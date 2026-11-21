
#include "../include/TimerValidator.h"
#include <cctype>
#include <string>

bool TimerValidator::IsValid(const std::string& timer)
{
    if (timer.size() != 5 || timer[2] != ':')
        return false;

    if (!std::isdigit(static_cast<unsigned char>(timer[0])) ||
        !std::isdigit(static_cast<unsigned char>(timer[1])) ||
        !std::isdigit(static_cast<unsigned char>(timer[3])) ||
        !std::isdigit(static_cast<unsigned char>(timer[4])))
        return false;

    int minute = std::stoi(timer.substr(0, 2));
    int second = std::stoi(timer.substr(3, 2));

    return minute <= 130 && second <= 59;
}

bool TimerValidator::IsSequential(
    const std::string& previous,
    const std::string& current)
{
    if (previous.empty())
        return true;

    if (!IsValid(previous) || !IsValid(current))
        return false;

    int pm = std::stoi(previous.substr(0, 2));
    int ps = std::stoi(previous.substr(3, 2));
    int cm = std::stoi(current.substr(0, 2));
    int cs = std::stoi(current.substr(3, 2));

    int diff = (cm * 60 + cs) - (pm * 60 + ps);

    return diff >= 0 && diff <= 5;
}

bool TimerValidator::IsNewMatch(
    const std::string& previous,
    const std::string& current)
{
    if (!IsValid(previous) || !IsValid(current))
        return false;

    int pm = std::stoi(previous.substr(0, 2));
    int ps = std::stoi(previous.substr(3, 2));
    int cm = std::stoi(current.substr(0, 2));
    int cs = std::stoi(current.substr(3, 2));

    int previousTotal = pm * 60 + ps;
    int currentTotal = cm * 60 + cs;

    // Flag a large backward reset as a candidate,
    // not as a confirmed new match.
    return previousTotal >= 10 * 60 &&
           currentTotal <= 10 * 60 &&
           previousTotal - currentTotal > 5 * 60;
}
