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
    const std::string &timer,
    const std::string &competition,
    const std::string &station,
    bool newMatch);

public:
    bool HasFinished() const;

    const MatchSession &GetSession() const;

    void Reset();

private:
    enum class TrackerState
    {
        IDLE,
        RUNNING,
        WAITING_END,
        FINISHED
    };

    TrackerState trackerState = TrackerState::IDLE;

private:
    SessionManager session;
    std::chrono::steady_clock::time_point endCandidateTime;

    
};