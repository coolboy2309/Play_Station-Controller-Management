
#include "../include/MatchTracker.h"
#include "../include/Logger.h"
#include "../include/UploadManager.h"

#include <cstdlib>

namespace
{
    int TimerToSeconds(const std::string &timer)
    {
        if (timer.size() != 5 || timer[2] != ':')
            return -1;

        try
        {
            int minutes = std::stoi(timer.substr(0, 2));
            int seconds = std::stoi(timer.substr(3, 2));

            if (seconds > 59)
                return -1;

            return minutes * 60 + seconds;
        }
        catch (...)
        {
            return -1;
        }
    }
}

void MatchTracker::Update(
    MatchState state,
    int minute,
    int second,
    const std::string &timer,
    const std::string &competition,
    const std::string &station,
    bool newMatch)
{
    int currentTotal = TimerToSeconds(timer);

    if (state == MatchState::EXTRA_TIME)
    {
        extraTimeDetected = true;
    }

    if (currentTotal < 0)
        return;

    //------------------------------------------------
    // IDLE
    //------------------------------------------------

    if (trackerState == TrackerState::IDLE)
    {
        if (state == MatchState::FIRST_HALF ||
            state == MatchState::SECOND_HALF)
        {
            trackerState = TrackerState::RUNNING;

            session.Start(
                competition,
                station,
                timer);

            lastKnownTimer = timer;

            Logger::Info("MATCH STARTED");
        }

        return;
    }

    //------------------------------------------------
    // Detect a possible new match
    //------------------------------------------------

    if (trackerState == TrackerState::RUNNING &&
        newMatch &&
        !pendingNewMatch)
    {
        pendingNewMatch = true;
        candidateReadings = 1;
        candidateStartSeconds = currentTotal;
        candidateLastSeconds = currentTotal;

        Logger::Info(
            "POSSIBLE NEW MATCH. Waiting for confirmation.");

        return;
    }

    //------------------------------------------------
    // Confirm or cancel a possible new match
    //------------------------------------------------

    if (pendingNewMatch)
    {
        int difference = currentTotal - candidateLastSeconds;

        bool candidateValid =
            currentTotal <= 10 * 60 &&
            difference >= 0 &&
            difference <= 60;

        if (candidateValid)
        {
            candidateReadings++;
            candidateLastSeconds = currentTotal;

            Logger::Info(
                "NEW MATCH CONFIRMATION: " +
                std::to_string(candidateReadings) +
                "/3");

            bool enoughProgress =
                currentTotal - candidateStartSeconds >= 5;

            if (candidateReadings >= 3 && enoughProgress)
            {
                Logger::Info("NEW MATCH CONFIRMED");

                // Preserve extra-time information.
                session.Finish(lastKnownTimer, extraTimeDetected);

                trackerState = TrackerState::FINISHED;
                pendingNewMatch = false;

                Logger::Info("OLD SESSION FINISHED");
                return;
            }

            return;
        }

        // The old match may have resumed, or the reset
        // may have been caused by a temporary interruption.
        pendingNewMatch = false;
        candidateReadings = 0;

        lastKnownTimer = timer;

        Logger::Info(
            "NEW MATCH CANDIDATE CANCELLED. Continuing session.");
    }

    //------------------------------------------------
    // RUNNING / WAITING_END
    //------------------------------------------------

    if (trackerState == TrackerState::RUNNING)
    {
        lastKnownTimer = timer;

        if (state == MatchState::HALFTIME)
        {
            Logger::Info("HALFTIME");
        }

        if (state == MatchState::END_CANDIDATE)
        {
            trackerState = TrackerState::WAITING_END;

            endCandidateTime =
                std::chrono::steady_clock::now();

            Logger::Info("WAITING FOR MATCH END");
        }

        return;
    }

    if (trackerState == TrackerState::WAITING_END)
    {
        if (state == MatchState::EXTRA_TIME)
        {
            trackerState = TrackerState::RUNNING;
            lastKnownTimer = timer;

            Logger::Info("EXTRA TIME DETECTED");
            return;
        }

        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() -
                endCandidateTime);

        Logger::Info(
            "END CANDIDATE WAIT: " +
            std::to_string(elapsed.count()) +
            " sec | Timer: " + timer);
        session.Finish(lastKnownTimer, false);
        if (elapsed.count() >= 20)
        {
            trackerState = TrackerState::FINISHED;

            session.Finish(timer, extraTimeDetected);

            Logger::Info("MATCH FINISHED");
        }
    }
}

bool MatchTracker::HasFinished() const
{
    return trackerState == TrackerState::FINISHED;
}

const MatchSession &MatchTracker::GetSession() const
{
    return session.GetSession();
}

void MatchTracker::Reset()
{
    trackerState = TrackerState::IDLE;

    pendingNewMatch = false;
    candidateReadings = 0;
    candidateStartSeconds = 0;
    candidateLastSeconds = 0;
    extraTimeDetected = false;

    lastKnownTimer.clear();
}
void MatchTracker::FinishByFullTime()
{
    if (trackerState == TrackerState::IDLE ||
        trackerState == TrackerState::FINISHED)
    {
        return;
    }

    if (lastKnownTimer.empty())
    {
        Logger::Error(
            "Cannot finish session: no valid timer available.");
        return;
    }

    session.Finish(lastKnownTimer, extraTimeDetected);

    trackerState = TrackerState::FINISHED;
    pendingNewMatch = false;

    Logger::Info("MATCH FINISHED BY FULL TIME DETECTION");
}
