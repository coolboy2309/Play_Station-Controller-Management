
#include "../include/HttpClient.h"
#include "../include/Logger.h"

#include <curl/curl.h>
#include <string>

// Safely escape strings for JSON.
static std::string EscapeJson(const std::string &value)
{
    std::string result;

    for (char c : value)
    {
        switch (c)
        {
        case '"':
            result += "\\\"";
            break;

        case '\\':
            result += "\\\\";
            break;

        case '\n':
            result += "\\n";
            break;

        case '\r':
            result += "\\r";
            break;

        case '\t':
            result += "\\t";
            break;

        default:
            result += c;
            break;
        }
    }

    return result;
}

bool HttpClient::SendHeartbeat(
    const std::string &serverURL,
    const std::string &agentID)
{
    CURL *curl = curl_easy_init();

    if (!curl)
    {
        Logger::Error("Cannot initialize CURL");
        return false;
    }

    std::string url =
        serverURL + "/agent/heartbeat";

    std::string json =
        "{\"agent_id\":\"" +
        EscapeJson(agentID) +
        "\"}";

    struct curl_slist *headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode result = curl_easy_perform(curl);

    long httpCode = 0;

    if (result == CURLE_OK)
    {
        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &httpCode);
    }

    bool success =
        result == CURLE_OK &&
        httpCode >= 200 &&
        httpCode < 300;

    if (success)
    {
        Logger::Info("Heartbeat Sent");
    }
    else if (result != CURLE_OK)
    {
        Logger::Error(curl_easy_strerror(result));
    }
    else
    {
        Logger::Error(
            "Heartbeat HTTP Error: " +
            std::to_string(httpCode));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return success;
}

bool HttpClient::SendSession(
    const std::string &serverURL,
    const MatchSession &session)
{
    CURL *curl = curl_easy_init();

    if (!curl)
    {
        Logger::Error("Cannot initialize CURL");
        return false;
    }

    std::string url =
        serverURL + "/session";

    std::string json =
        "{"
        "\"session_id\":\"" +
        EscapeJson(session.sessionID) +
        "\","
        "\"competition\":\"" +
        EscapeJson(session.competition) +
        "\","
        "\"station\":\"" +
        EscapeJson(session.station) +
        "\","
        "\"start_timer\":\"" +
        EscapeJson(session.startTimer) +
        "\","
        "\"end_timer\":\"" +
        EscapeJson(session.endTimer) +
        "\","
        "\"extra_time\":" +
        std::string(session.extraTime ? "true" : "false") +
        "}";

    struct curl_slist *headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        json.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        static_cast<long>(json.size()));

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Network timeouts.
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

    CURLcode result = curl_easy_perform(curl);

    long httpCode = 0;

    if (result == CURLE_OK)
    {
        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &httpCode);
    }

    bool success =
        result == CURLE_OK &&
        httpCode >= 200 &&
        httpCode < 300;

    if (success)
    {
        Logger::Info(
            "SESSION SENT - HTTP " +
            std::to_string(httpCode));
    }
    else if (result != CURLE_OK)
    {
        Logger::Error(
            "Session Upload Failed: " +
            std::string(curl_easy_strerror(result)));
    }
    else
    {
        Logger::Error(
            "Session Upload HTTP Error: " +
            std::to_string(httpCode));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return success;
}
