#include "../include/Config.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool Config::Load(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "Cannot open config file\n";
        return false;
    }

    json j;

    file >> j;

    agentID = j["agent_id"];
    stationName = j["station_name"];
    serverURL = j["server_url"];

    heartbeatInterval = j["heartbeat_interval"];
    captureInterval = j["capture_interval"];

    return true;
}