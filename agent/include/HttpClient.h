#pragma once

#include <string>

class HttpClient
{
public:

    bool SendHeartbeat(
        const std::string& serverURL,
        const std::string& agentID
    );

};