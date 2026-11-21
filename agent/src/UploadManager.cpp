
#include "../include/UploadManager.h"
#include "../include/Logger.h"

UploadManager::UploadManager(
    const std::string& serverURL)
{
    server = serverURL;
}

bool UploadManager::UploadSession(
    const MatchSession& session)
{
    Logger::Info("Uploading Match Session...");

    bool success = client.SendSession(
        server,
        session);

    if (!success)
    {
        Logger::Error(
            "Session upload failed. Session retained for retry.");

        return false;
    }

    Logger::Info("Session Upload Confirmed");

    return true;
}
