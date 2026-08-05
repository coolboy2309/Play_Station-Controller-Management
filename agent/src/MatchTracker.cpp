#include "../include/MatchTracker.h"
#include "../include/Logger.h"
#include "../include/UploadManager.h"

void MatchTracker::Update(
    MatchState state,
    int minute,
    int second,
    const std::string &timer,
    const std::string &competition,
    const std::string &station)
{
    switch (trackerState)
    {
    case TrackerState::IDLE:

        if (state == MatchState::FIRST_HALF ||
            state == MatchState::SECOND_HALF)
        {
            trackerState = TrackerState::RUNNING;

            session.Start(
                competition,
                station,
                timer);

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

            endCandidateTime = std::chrono::steady_clock::now();

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

        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() -
                endCandidateTime);

        if (elapsed.count() >= 10)
        {
            trackerState = TrackerState::FINISHED;

            session.Finish(
                timer,
                state == MatchState::EXTRA_TIME);

            UploadManager uploader;

            uploader.UploadSession(
                session.GetSession());

            Logger::Info("MATCH FINISHED");
        }

        break;

    case TrackerState::FINISHED:

        break;
    }
}