#pragma once

#include "HttpClient.h"
#include "Config.h"

class HeartbeatManager
{
public:

    HeartbeatManager(Config& config);

    void Update();

private:

    Config& config;

    HttpClient client;

    int counter = 0;

};