#include "../include/MatchSession.h"

MatchSession::MatchSession()
{
    stationName = "";

    startTime = "";

    currentMinute = 0;

    currentSecond = 0;

    state = MatchState::UNKNOWN;

    running = false;

    revenue = 0.0;
}