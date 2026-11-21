
#pragma once

#include "MatchState.h"
#include "SessionManager.h"

#include <chrono>
#include <string>

class MatchTracker
{
public:
    void Update(
        MatchState state,
        int minute,
        int second,
        const std::string& timer,
        const std::string& competition,
        const std::string& station,
        bool newMatch);

    bool HasFinished() const;

    const MatchSession& GetSession() const;

    void Reset();

    // Close the active session when FULL TIME is confirmed.
    void FinishByFullTime();

private:
    enum class TrackerState
    {
        IDLE,
        RUNNING,
        WAITING_END,
        FINISHED
    };

    TrackerState trackerState = TrackerState::IDLE;

    SessionManager session;

    std::chrono::steady_clock::time_point endCandidateTime;

    // New-match confirmation
    bool pendingNewMatch = false;
    int candidateReadings = 0;
    int candidateStartSeconds = 0;
    int candidateLastSeconds = 0;

    // Last known timer from the existing match
    std::string lastKnownTimer;

    // Tracks whether extra time was detected
    bool extraTimeDetected = false;
};
