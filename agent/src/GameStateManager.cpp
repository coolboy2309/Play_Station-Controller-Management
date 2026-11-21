#include "../include/GameStateManager.h"

MatchState GameStateManager::Detect(int minute)
{
    if (minute < 0)
        return MatchState::UNKNOWN;

    switch (currentState)
    {
    case MatchState::UNKNOWN:

        if (minute < 45)
            currentState = MatchState::FIRST_HALF;
        else if (minute <= 45)
            currentState = MatchState::HALFTIME;
        else if (minute <= 95)
            currentState = MatchState::SECOND_HALF;
        else if (minute >= 100)
            currentState = MatchState::EXTRA_TIME;

        break;

    case MatchState::FIRST_HALF:

        if (minute >= 45 && minute < 90)
            currentState = MatchState::HALFTIME;

        break;

    case MatchState::HALFTIME:

        if (minute >= 46 && minute < 90)
            currentState = MatchState::SECOND_HALF;

        break;

    case MatchState::SECOND_HALF:

        if (minute >= 90 && minute <= 95)
            currentState = MatchState::END_CANDIDATE;

        else if (minute >= 100)
            currentState = MatchState::EXTRA_TIME;

        break;

    case MatchState::END_CANDIDATE:

        if (minute >= 100)
            currentState = MatchState::EXTRA_TIME;

        break;

    case MatchState::EXTRA_TIME:

        break;

    case MatchState::MATCH_FINISHED:

        break;
    }

    return currentState;
}