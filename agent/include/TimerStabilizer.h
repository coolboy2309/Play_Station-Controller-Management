#pragma once

#include <string>

class TimerStabilizer
{
public:

    std::string Update(const std::string& timer);

private:

    std::string lastValidTimer = "";

    int badFrames = 0;
};