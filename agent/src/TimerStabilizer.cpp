
#include "../include/TimerStabilizer.h"

std::string TimerStabilizer::Update(const std::string& timer)
{
    // Handle failed OCR readings.
    if (timer.empty())
    {
        emptyReadings++;

        if (emptyReadings > MAX_EMPTY_READINGS)
        {
            history.clear();
            return "";
        }

        if (history.empty())
            return "";

        return history.back();
    }

    // Accept the newest non-empty OCR reading.
    emptyReadings = 0;

    history.clear();
    history.push_back(timer);

    return timer;
}
