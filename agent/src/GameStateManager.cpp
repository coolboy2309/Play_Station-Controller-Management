#include "../include/GameStateManager.h"

MatchState GameStateManager::Detect(int minute)
{
    if (minute < 45)
        return MatchState::FIRST_HALF;

    if (minute == 45)
        return MatchState::HALFTIME;

    if (minute > 45 && minute < 90)
        return MatchState::SECOND_HALF;

    if (minute == 90)
        return MatchState::END_CANDIDATE;

    if (minute > 90)
        return MatchState::EXTRA_TIME;

    return MatchState::UNKNOWN;
}