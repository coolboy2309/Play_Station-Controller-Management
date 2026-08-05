#include "../include/UploadManager.h"
#include "../include/Logger.h"

void UploadManager::Update()
{
    Logger::Info("Upload Tick");
}

void UploadManager::UploadSession(
    const MatchSession& session)
{
    Logger::Info("=========== SESSION TO SEND ===========");

    Logger::Info("Competition : " + session.competition);

    Logger::Info("Station : " + session.station);

    Logger::Info("Start : " + session.startTimer);

    Logger::Info("End : " + session.endTimer);

    Logger::Info(
        std::string("Extra Time : ") +
        (session.extraTime ? "YES" : "NO"));

    Logger::Info("=======================================");
}