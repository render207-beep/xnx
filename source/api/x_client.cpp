#include "api/x_client.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

// Busca recursivamente nodos "tweet_results" en la respuesta de GraphQL.
// La forma exacta cambia con el tiempo, por eso es tolerante.
static void collect(const json& n, std::vector<Tweet>& out) {
    if (n.is_object()) {
        auto it = n.find("tweet_results");
        if (it != n.end() && it->contains("result")) {
            json res = (*it)["result"];
            if (res.contains("tweet")) res = res["tweet"]; // TweetWithVisibilityResults
            Tweet t;
            if (res.contains("rest_id")) t.id = res["rest_id"].get<std::string>();
            if (res.contains("legacy"))
                t.text = res["legacy"].value("full_text", "");
            try {
                const json& u = res["core"]["user_results"]["result"];
                if (u.contains("core")) {            // formato nuevo
                    t.name = u["core"].value("name", "");
                    t.handle = u["core"].value("screen_name", "");
                } else if (u.contains("legacy")) {   // formato anterior
                    t.name = u["legacy"].value("name", "");
                    t.handle = u["legacy"].value("screen_name", "");
                }
            } catch (...) {}
            if (!t.id.empty() && !t.text.empty()) out.push_back(std::move(t));
        }
        for (auto& [k, v] : n.items()) collect(v, out);
    } else if (n.is_array()) {
        for (auto& v : n) collect(v, out);
    }
}

static std::string urlEncode(const std::string& s) {
    static const char* hex = "0123456789ABCDEF";
    std::string o;
    for (unsigned char c : s) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') o += c;
        else { o += '%'; o += hex[c >> 4]; o += hex[c & 15]; }
    }
    return o;
}

TimelineResult XClient::homeTimeline(int count) {
    TimelineResult out;
    if (cfg.authToken.empty() || cfg.ct0.empty() || cfg.bearer.empty() ||
        cfg.homeTimelineQueryId.empty()) {
        out.error = "Falta config: auth_token, ct0, bearer o home_timeline_query_id "
                    "en sdmc:/switch/xnx/config.json";
        return out;
    }

    json vars = {{"count", count}, {"includePromotedContent", false},
                 {"latestControlAvailable", true}, {"requestContext", "launch"}};
    // OJO: X exige además un JSON "features" cuyo contenido cambia; cópialo
    // del request real que hace x.com (DevTools > Network) a config si hace falta.
    json features = json::object();

    std::string url = "https://x.com/i/api/graphql/" + cfg.homeTimelineQueryId +
                      "/HomeTimeline?variables=" + urlEncode(vars.dump()) +
                      "&features=" + urlEncode(features.dump());

    auto r = http.get(url, {
        {"authorization", "Bearer " + cfg.bearer},
        {"x-csrf-token", cfg.ct0},
        {"cookie", "auth_token=" + cfg.authToken + "; ct0=" + cfg.ct0},
        {"x-twitter-auth-type", "OAuth2Session"},
        {"x-twitter-active-user", "yes"},
        {"user-agent", "Mozilla/5.0 (X11; Linux aarch64) AppleWebKit/537.36 "
                       "(KHTML, like Gecko) Chrome/130 Safari/537.36"},
    });

    if (!r.error.empty()) { out.error = "Red: " + r.error; return out; }
    if (r.status != 200) { out.error = "HTTP " + std::to_string(r.status); return out; }

    try { collect(json::parse(r.body), out.tweets); }
    catch (const std::exception& e) { out.error = std::string("JSON: ") + e.what(); }
    return out;
}
