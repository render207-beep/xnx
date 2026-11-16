#include "config.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

Config Config::load(const std::string& path) {
    Config c;
    std::ifstream f(path);
    if (!f) return c;
    try {
        auto j = nlohmann::json::parse(f);
        c.authToken = j.value("auth_token", "");
        c.ct0 = j.value("ct0", "");
        c.bearer = j.value("bearer", "");
        c.homeTimelineQueryId = j.value("home_timeline_query_id", "");
        c.insecureTls = j.value("insecure_tls", false);
    } catch (...) {}
    return c;
}
