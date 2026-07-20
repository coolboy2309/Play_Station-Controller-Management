#include "../include/Application.h"
#include "../include/Logger.h"
#include "../include/HeartbeatManager.h"
#include "../include/CaptureManager.h"
#include "../include/UploadManager.h"
#include "../include/OCRManager.h"
#include "../include/GameStateParser.h"
#include "../include/GameStateManager.h"
#include "../include/MatchTracker.h"

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
    UploadManager upload;
    MatchTracker tracker;

    while (true)
    {
        heartbeat.Update();
        capture.Capture();
        OCRManager ocr;

        std::string timer = ocr.ReadTimer("timer_binary.jpg");

        Logger::Info("OCR : " + timer);

        int minute = GameStateParser::GetMinute(timer);

        int second = GameStateParser::GetSecond(timer);

        MatchState state = GameStateManager::Detect(minute);

        tracker.Update(state, minute, second);

        Logger::Info("Minute : " + std::to_string(minute));

        Logger::Info("Second : " + std::to_string(second));

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
            second);

        upload.Update();

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }
}