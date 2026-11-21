
#include "../include/SessionManager.h"
#include "../include/Logger.h"

#include <array>
#include <random>
#include <sstream>
#include <iomanip>

static std::string GenerateSessionID()
{
    std::array<unsigned char, 16> bytes{};

    static std::random_device rd;

    for (auto& byte : bytes)
    {
        byte = static_cast<unsigned char>(rd());
    }

    // UUID version 4
    bytes[6] = (bytes[6] & 0x0F) | 0x40;

    // UUID variant
    bytes[8] = (bytes[8] & 0x3F) | 0x80;

    std::ostringstream result;

    result << std::hex << std::setfill('0');

    for (int i = 0; i < 16; ++i)
    {
        result << std::setw(2)
               << static_cast<int>(bytes[i]);

        if (i == 3 || i == 5 || i == 7 || i == 9)
            result << "-";
    }

    return result.str();
}

void SessionManager::Start(
    const std::string& comp,
    const std::string& st,
    const std::string& timer)
{
    if (active)
        return;

    // Reset previous session data.
    session = MatchSession{};

    active = true;

    session.sessionID = GenerateSessionID();
    session.competition = comp;
    session.station = st;
    session.startTimer = timer;
    session.startTime = std::chrono::steady_clock::now();

    Logger::Info("SESSION CREATED");
    Logger::Info("Session ID : " + session.sessionID);
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

    Logger::Info("Session ID : " + session.sessionID);
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
