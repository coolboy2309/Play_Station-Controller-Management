
#include "../include/Application.h"
#include "../include/Logger.h"
#include "../include/HeartbeatManager.h"
#include "../include/CaptureManager.h"
#include "../include/UploadManager.h"
#include "../include/OCRManager.h"
#include "../include/FullTimeDetector.h"
#include "../include/GameStateParser.h"
#include "../include/GameStateManager.h"
#include "../include/MatchTracker.h"
#include "../include/TimerValidator.h"

#include <thread>
#include <chrono>
#include <string>

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
    UploadManager upload(config.serverURL);

    MatchTracker tracker;
    GameStateManager stateManager;
    OCRManager ocr;
    FullTimeDetector fullTimeDetector;

    static std::string previousTimer;

    while (true)
    {
        auto loopStart = std::chrono::steady_clock::now();

        heartbeat.Update();

        //------------------------------------------------
        // 1. Capture Screen
        //------------------------------------------------

        cv::Mat grayImage;
        cv::Mat binaryImage;

        auto captureStart = std::chrono::steady_clock::now();

        bool captureSuccess = capture.Capture(
            "",
            config.competition,
            grayImage,
            binaryImage);

        auto captureEnd = std::chrono::steady_clock::now();

        Logger::Info(
            "CAPTURE TIME: " +
            std::to_string(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    captureEnd - captureStart)
                    .count()) +
            " ms");

        //------------------------------------------------
        // 2. Full Screen Detection
        //------------------------------------------------

        cv::Mat fullFrame = capture.GetLastFrame();

        bool fullTimeConfirmed = false;

        if (!fullFrame.empty())
        {
            fullTimeConfirmed =
                fullTimeDetector.Detect(fullFrame);
        }
        else
        {
            Logger::Error("Full screen frame unavailable.");
        }

        //------------------------------------------------
        // 3. Finish Match if Full Time Confirmed
        //------------------------------------------------

        if (fullTimeConfirmed)
        {
            Logger::Info("FULL TIME EVENT DETECTED");

            tracker.FinishByFullTime();
        }

        //------------------------------------------------
        // 4. Timer OCR
        //------------------------------------------------

        if (captureSuccess && !fullTimeConfirmed)
        {
            std::string competition =
                capture.GetCompetition();

            auto ocrStart = std::chrono::steady_clock::now();

            std::string timer = ocr.ReadTimer(
                grayImage,
                binaryImage);

            auto ocrEnd = std::chrono::steady_clock::now();

            Logger::Info(
                "OCR TIME: " +
                std::to_string(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        ocrEnd - ocrStart)
                        .count()) +
                " ms");

            Logger::Info("OCR : " + timer);

            //------------------------------------------------
            // 5. Timer Validation
            //------------------------------------------------

            bool validTimer = TimerValidator::IsValid(timer);
            bool newMatch = false;

            if (validTimer)
            {
                newMatch = TimerValidator::IsNewMatch(
                    previousTimer,
                    timer);

                previousTimer = timer;
            }

            //------------------------------------------------
            // 6. Game State Detection
            //------------------------------------------------

            if (validTimer)
            {
                int minute = GameStateParser::GetMinute(timer);
                int second = GameStateParser::GetSecond(timer);

                MatchState state =
                    stateManager.Detect(minute);

                Logger::Info(
                    "Minute : " + std::to_string(minute));

                Logger::Info(
                    "Second : " + std::to_string(second));

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

                //------------------------------------------------
                // 7. Match Tracking
                //------------------------------------------------

                tracker.Update(
                    state,
                    minute,
                    second,
                    timer,
                    competition,
                    config.stationName,
                    newMatch);
            }
            else
            {
                Logger::Info(
                    "Invalid timer reading. Skipping state update.");
            }
        }
        else if (!captureSuccess)
        {
            Logger::Info(
                "Timer processing skipped. Full screen detection already checked.");
        }

        //------------------------------------------------
        // 8. Upload Finished Session
        //------------------------------------------------

        if (tracker.HasFinished())
        {
            bool uploadSuccess =
                upload.UploadSession(tracker.GetSession());

            if (uploadSuccess)
            {
                tracker.Reset();

                Logger::Info(
                    "Session Upload Completed. Tracker Reset.");
            }
            else
            {
                Logger::Error(
                    "Session retained. Upload will be retried.");
            }
        }

        //------------------------------------------------
        // 9. Loop Performance
        //------------------------------------------------

        auto loopEnd = std::chrono::steady_clock::now();

        Logger::Info(
            "TOTAL LOOP TIME: " +
            std::to_string(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    loopEnd - loopStart)
                    .count()) +
            " ms");

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }
}
