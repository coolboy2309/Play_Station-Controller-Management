#include "../include/SessionManager.h"

void SessionManager::Start(MatchSession& session)
{
    session.running = true;
}

void SessionManager::Update(
    MatchSession& session,
    int minute,
    int second,
    MatchState state)
{
    session.currentMinute = minute;

    session.currentSecond = second;

    session.state = state;
}

void SessionManager::Finish(MatchSession& session)
{
    session.running = false;
}