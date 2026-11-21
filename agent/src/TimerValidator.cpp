#include "../include/TimerValidator.h"
#include <cctype>
#include <string>

bool TimerValidator::IsValid(const std::string& timer)
{
    if (timer.size() != 5)
        return false;

    if (timer[2] != ':')
        return false;

    if (!std::isdigit(static_cast<unsigned char>(timer[0])) ||
        !std::isdigit(static_cast<unsigned char>(timer[1])) ||
        !std::isdigit(static_cast<unsigned char>(timer[3])) ||
        !std::isdigit(static_cast<unsigned char>(timer[4])))
    {
        return false;
    }

    int minute =
        std::stoi(timer.substr(0, 2));

    int second =
        std::stoi(timer.substr(3, 2));

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
    if (previous.empty())
        return true;

    if (!IsValid(previous) ||
        !IsValid(current))
    {
        return false;
    }

    int pm =
        std::stoi(previous.substr(0, 2));

    int ps =
        std::stoi(previous.substr(3, 2));

    int cm =
        std::stoi(current.substr(0, 2));

    int cs =
        std::stoi(current.substr(3, 2));

    int previousTotal =
        pm * 60 + ps;

    int currentTotal =
        cm * 60 + cs;

    int diff =
        currentTotal - previousTotal;

    if (diff < 0)
        return false;

    if (diff > 5)
        return false;

    return true;
}

bool TimerValidator::IsNewMatch(
    const std::string& previous,
    const std::string& current)
{
    if (previous.empty() || current.empty())
        return false;

    if (!IsValid(previous) || !IsValid(current))
        return false;

    int previousMinute =
        std::stoi(previous.substr(0, 2));

    int previousSecond =
        std::stoi(previous.substr(3, 2));

    int currentMinute =
        std::stoi(current.substr(0, 2));

    int currentSecond =
        std::stoi(current.substr(3, 2));

    int previousTotal =
        previousMinute * 60 + previousSecond;

    int currentTotal =
        currentMinute * 60 + currentSecond;

    // Match has reached the normal end area
    // and the timer suddenly returned near 00:00.
    if (previousTotal >= 80 * 60 &&
        currentTotal <= 10 * 60)
    {
        return true;
    }

    return false;
}