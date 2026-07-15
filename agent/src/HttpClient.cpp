#include "../include/HttpClient.h"
#include "../include/Logger.h"

#include <curl/curl.h>

bool HttpClient::SendHeartbeat(
    const std::string& serverURL,
    const std::string& agentID
)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        Logger::Error("Cannot initialize CURL");
        return false;
    }

    std::string url =
        serverURL +
        "/agent/heartbeat";

    std::string json =
        "{\"agent_id\":\"" +
        agentID +
        "\"}";

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        json.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    CURLcode result =
        curl_easy_perform(curl);

    if(result == CURLE_OK)
    {
        Logger::Info("Heartbeat Sent");
    }
    else
    {
        Logger::Error(curl_easy_strerror(result));
    }

    curl_slist_free_all(headers);

    curl_easy_cleanup(curl);

    return result == CURLE_OK;
}