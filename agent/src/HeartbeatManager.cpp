#include "../include/HeartbeatManager.h"

HeartbeatManager::HeartbeatManager(Config& cfg)
    : config(cfg)
{
}

void HeartbeatManager::Update()
{
    counter++;

    if(counter >= 10)
    {
        client.SendHeartbeat(
            config.serverURL,
            config.agentID
        );

        counter = 0;
    }
}