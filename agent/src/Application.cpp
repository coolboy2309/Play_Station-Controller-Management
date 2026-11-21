#include "../include/Application.h"
#include "../include/Logger.h"
#include "../include/HeartbeatManager.h"
#include "../include/CaptureManager.h"
#include "../include/UploadManager.h"
#include "../include/OCRManager.h"
#include "../include/GameStateParser.h"
#include "../include/GameStateManager.h"
#include "../include/MatchTracker.h"
#include "../include/TimerValidator.h"

#include <thread>
#include <chrono>

bool Application::Initialize()
{
    Logger::Info("ZoneEye Agent Starting...");

    if (!config.Load("../config/config.ini"))
    {
        Logger::Error("Cannot Load Configuration");

        return false;
    }

    Logger::Info("Configuration Loaded");

    Logger::Info("Agent ID : " + config.agentID);

    Logger::Info("Station  : " + config.stationName);

    Logger::Info("Server   : " + config.serverURL);

    return true;
}

void Application::Run()
{
    Logger::Info("Main Loop Started");

    HeartbeatManager heartbeat(config);
    CaptureManager capture;
    UploadManager upload(
        config.serverURL);
    MatchTracker tracker;
    GameStateManager stateManager;

    while (true)
{
    auto loopStart = std::chrono::steady_clock::now();

    heartbeat.Update();

    auto captureStart = std::chrono::steady_clock::now();

    capture.Capture(
        "",
        config.competition);

    auto captureEnd = std::chrono::steady_clock::now();

    Logger::Info(
        "CAPTURE TIME: " +
        std::to_string(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                captureEnd - captureStart
            ).count()
        ) +
        " ms"
    );

    std::string competition =
        capture.GetCompetition();

    OCRManager ocr;

    auto ocrStart = std::chrono::steady_clock::now();

    std::string timer =
        ocr.ReadTimer();

    auto ocrEnd = std::chrono::steady_clock::now();

    Logger::Info(
        "OCR TIME: " +
        std::to_string(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                ocrEnd - ocrStart
            ).count()
        ) +
        " ms"
    );

    static std::string previousTimer;

    bool newMatch =
        TimerValidator::IsNewMatch(
            previousTimer,
            timer);

    previousTimer = timer;

    Logger::Info("OCR : " + timer);

    int minute =
        GameStateParser::GetMinute(timer);

    int second =
        GameStateParser::GetSecond(timer);

    MatchState state =
        stateManager.Detect(minute);

    Logger::Info(
        "Minute : " +
        std::to_string(minute));

    Logger::Info(
        "Second : " +
        std::to_string(second));

    switch (state)
    {
    case MatchState::FIRST_HALF:
        Logger::Info("State : FIRST_HALF");
        break;

    case MatchState::HALFTIME:
        Logger::Info("State : HALFTIME");
        break;

    case MatchState::SECOND_HALF:
        Logger::Info("State : SECOND_HALF");
        break;

    case MatchState::END_CANDIDATE:
        Logger::Info("State : END_CANDIDATE");
        break;

    case MatchState::EXTRA_TIME:
        Logger::Info("State : EXTRA_TIME");
        break;

    default:
        Logger::Info("State : UNKNOWN");
        break;
    }

    tracker.Update(
        state,
        minute,
        second,
        timer,
        competition,
        config.stationName,
        newMatch);

    if (tracker.HasFinished())
    {
        upload.UploadSession(
            tracker.GetSession());

        tracker.Reset();
    }

    auto loopEnd = std::chrono::steady_clock::now();

    Logger::Info(
        "TOTAL LOOP TIME: " +
        std::to_string(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                loopEnd - loopStart
            ).count()
        ) +
        " ms"
    );

    std::this_thread::sleep_for(
        std::chrono::seconds(1));
}
}