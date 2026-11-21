#pragma once

#include "MatchState.h"
#include <chrono>

class GameStateManager
{
public:

    MatchState Detect(int minute);

    void Reset();

private:

    MatchState currentState = MatchState::UNKNOWN;

    bool waitingForEnd = false;

    std::chrono::steady_clock::time_point endCandidateTime;
};