#pragma once

#include "MatchSession.h"

class UploadManager
{
public:
    void Update();

    void UploadSession(const MatchSession& session);
};