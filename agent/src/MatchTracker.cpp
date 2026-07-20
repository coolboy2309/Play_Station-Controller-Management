#include "../include/MatchTracker.h"
#include "../include/Logger.h"

void MatchTracker::Update(
    MatchState state,
    int minute,
    int second)
{
    switch (trackerState)
    {
    case TrackerState::IDLE:

        if (state == MatchState::FIRST_HALF ||
            state == MatchState::SECOND_HALF)
        {
            trackerState = TrackerState::RUNNING;

            Logger::Info("MATCH STARTED");
        }

        break;

    case TrackerState::RUNNING:

        if (state == MatchState::HALFTIME)
        {
            Logger::Info("HALFTIME");
        }

        if (state == MatchState::END_CANDIDATE)
        {
            trackerState = TrackerState::WAITING_END;

            endCandidateSecond = second;

            Logger::Info("WAITING FOR MATCH END");
        }

        break;

    case TrackerState::WAITING_END:

        if (state == MatchState::EXTRA_TIME)
        {
            trackerState = TrackerState::RUNNING;

            Logger::Info("EXTRA TIME DETECTED");

            break;
        }

        if ((second - endCandidateSecond) >= 10)
        {
            trackerState = TrackerState::FINISHED;

            Logger::Info("MATCH FINISHED");
        }

        break;

    case TrackerState::FINISHED:

        break;
    }
}