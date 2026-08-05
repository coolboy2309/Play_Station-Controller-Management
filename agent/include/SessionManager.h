#pragma once

#include "MatchSession.h"

class SessionManager
{
public:

    void Start(
        const std::string& competition,
        const std::string& station,
        const std::string& timer);

    void Finish(
        const std::string& timer,
        bool extraTime);

    const MatchSession& GetSession() const;

private:

    bool active = false;

    MatchSession session;
};