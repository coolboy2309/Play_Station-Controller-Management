#pragma once

#include <string>

class Config
{
public:

    std::string agentID;
    std::string stationName;
    std::string serverURL;
    std::string competition;

    // NEW
    std::string mode;

    int heartbeatInterval;
    int captureInterval;

    bool Load(const std::string& path);
};