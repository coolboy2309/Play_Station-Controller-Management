#include "../include/TimerStabilizer.h"
#include "../include/GameStateParser.h"

std::string TimerStabilizer::Update(const std::string& timer)
{
    if (timer.empty())
        return lastValidTimer;

    if (lastValidTimer.empty())
    {
        lastValidTimer = timer;
        return timer;
    }

    int oldMinute =
        GameStateParser::GetMinute(lastValidTimer);

    int oldSecond =
        GameStateParser::GetSecond(lastValidTimer);

    int newMinute =
        GameStateParser::GetMinute(timer);

    int newSecond =
        GameStateParser::GetSecond(timer);

    if (oldMinute < 0 ||
        newMinute < 0)
    {
        return lastValidTimer;
    }

    int oldTotal =
        oldMinute * 60 + oldSecond;

    int newTotal =
        newMinute * 60 + newSecond;

    int diff =
        newTotal - oldTotal;

    // Accept normal movement
    if (diff >= 0 && diff <= 2)
    {
        lastValidTimer = timer;
        badFrames = 0;
        return timer;
    }

    // Reject impossible jump
    badFrames++;

    return lastValidTimer;
}