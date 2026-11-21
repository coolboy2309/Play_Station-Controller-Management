
#pragma once

#include "HttpClient.h"
#include "MatchSession.h"
#include <string>

class UploadManager
{
public:

    UploadManager(
        const std::string& serverURL);

    bool UploadSession(
        const MatchSession& session);

private:

    HttpClient client;

    std::string server;
};
