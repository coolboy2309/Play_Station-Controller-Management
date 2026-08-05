#pragma once

#include <string>
#include "../include/MatchSession.h"

class HttpClient
{
public:

    bool SendHeartbeat(
        const std::string& serverURL,
        const std::string& agentID
    );

    bool SendSession(
        const std::string& serverURL,
        const MatchSession& session
    );
};