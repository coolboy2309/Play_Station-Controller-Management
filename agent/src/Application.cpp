#include "../include/Application.h"
#include "../include/Logger.h"
#include "../include/HeartbeatManager.h"
#include "../include/CaptureManager.h"
#include "../include/UploadManager.h"
#include "../include/OCRManager.h"

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

    while (true)
    {
        heartbeat.Update();
        capture.Capture();
        upload.Update();

        OCRManager ocr;

        std::string timer =
            ocr.ReadTimer("timer_crop.jpg");

        Logger::Info("OCR : " + timer);

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }
}