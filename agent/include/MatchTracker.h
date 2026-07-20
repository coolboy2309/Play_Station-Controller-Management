#pragma once

#include "MatchState.h"

class MatchTracker
{
public:
    void Update(MatchState state, int minute, int second);

private:
    enum class TrackerState
    {
        IDLE,
        RUNNING,
        WAITING_END,
        FINISHED
    };

    TrackerState trackerState = TrackerState::IDLE;

    int endCandidateSecond = 0;
};