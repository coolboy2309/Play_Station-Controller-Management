#include "../include/GameStateManager.h"

MatchState GameStateManager::Detect(int minute)
{
    switch (currentState)
    {
    case MatchState::UNKNOWN:

        if (minute >= 0)
            currentState = MatchState::FIRST_HALF;

        break;

    case MatchState::FIRST_HALF:

        if (minute >= 45)
            currentState = MatchState::HALFTIME;

        break;

    case MatchState::HALFTIME:

        if (minute >= 46)
            currentState = MatchState::SECOND_HALF;

        break;

    case MatchState::SECOND_HALF:

        if (minute >= 100)
            currentState = MatchState::EXTRA_TIME;

        break;

    default:
        break;
    }

    return currentState;
}