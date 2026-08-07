#include "../include/UploadManager.h"
#include "../include/Logger.h"

UploadManager::UploadManager(
    const std::string& serverURL)
{
    server = serverURL;
}

void UploadManager::UploadSession(
    const MatchSession& session)
{
    Logger::Info("Uploading Match Session...");

    client.SendSession(
        server,
        session);
}