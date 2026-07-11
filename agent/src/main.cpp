#include "../include/Config.h"
#include "../include/Logger.h"

int main()
{
    Logger::Info("ZoneEye Agent Starting...");

    Config config;

    if (!config.Load("../config/config.json"))
    {
        Logger::Error("Configuration Load Failed");
        return -1;
    }

    Logger::Info("Configuration Loaded");

    Logger::Info("Agent ID : " + config.agentID);

    Logger::Info("Station  : " + config.stationName);

    Logger::Info("Server   : " + config.serverURL);

    Logger::Info("Heartbeat: " + std::to_string(config.heartbeatInterval));

    Logger::Info("Capture  : " + std::to_string(config.captureInterval));

    Logger::Info("Agent Ready");

    while (true)
    {

    }

    return 0;
}