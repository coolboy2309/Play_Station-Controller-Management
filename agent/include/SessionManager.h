#pragma once

#include "MatchSession.h"

class SessionManager
{
public:

    void Start(MatchSession& session);

    void Update(
        MatchSession& session,
        int minute,
        int second,
        MatchState state);

    void Finish(MatchSession& session);
};