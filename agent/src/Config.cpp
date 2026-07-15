#include "../include/Config.h"

#include <fstream>
#include <iostream>


bool Config::Load(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);

        std::string value = line.substr(pos + 1);

        if (key == "agent_id")
            agentID = value;

        else if (key == "station_name")
            stationName = value;

        else if (key == "server_url")
            serverURL = value;

        else if (key == "heartbeat_interval")
            heartbeatInterval = std::stoi(value);

        else if (key == "capture_interval")
            captureInterval = std::stoi(value);
    }

    return true;
}