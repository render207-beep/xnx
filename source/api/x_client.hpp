#pragma once
#include <string>
#include <vector>
#include "api/http_client.hpp"
#include "config.hpp"

struct Tweet {
    std::string id;
    std::string name;
    std::string handle;
    std::string text;
};

struct TimelineResult {
    std::vector<Tweet> tweets;
    std::string error; // vacío si todo salió bien
};

class XClient {
public:
    explicit XClient(const Config& cfg) : cfg(cfg), http(cfg.insecureTls) {}
    TimelineResult homeTimeline(int count = 20);
private:
    Config cfg;
    HttpClient http;
};
