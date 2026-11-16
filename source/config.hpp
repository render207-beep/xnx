#pragma once
#include <string>

// Se lee de sdmc:/switch/xnx/config.json
struct Config {
    std::string authToken;   // cookie auth_token de x.com
    std::string ct0;         // cookie ct0 (también va como x-csrf-token)
    std::string bearer;      // Bearer público que usa el cliente web de X
    std::string homeTimelineQueryId; // ID del GraphQL "HomeTimeline"
    bool insecureTls = false;        // solo para depurar si falta cacert
    static Config load(const std::string& path);
};
