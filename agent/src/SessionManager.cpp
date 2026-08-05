#include "../include/SessionManager.h"
#include "../include/Logger.h"

void SessionManager::Start(
    const std::string& comp,
    const std::string& st,
    const std::string& timer)
{
    if (active)
        return;

    active = true;

    session.competition = comp;
    session.station = st;
    session.startTimer = timer;
    session.startTime = std::chrono::steady_clock::now();

    Logger::Info("SESSION CREATED");
}

void SessionManager::Finish(
    const std::string& timer,
    bool extra)
{
    if (!active)
        return;

    session.endTimer = timer;
    session.extraTime = extra;
    session.endTime = std::chrono::steady_clock::now();

    Logger::Info("SESSION CLOSED");

    Logger::Info("Competition : " + session.competition);
    Logger::Info("Station : " + session.station);
    Logger::Info("Start Timer : " + session.startTimer);
    Logger::Info("End Timer : " + session.endTimer);

    Logger::Info(
        std::string("Extra Time : ") +
        (extra ? "YES" : "NO"));

    active = false;
}

const MatchSession& SessionManager::GetSession() const
{
    return session;
}