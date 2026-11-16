#pragma once
#include <borealis.hpp>
#include "api/x_client.hpp"

class TimelineView : public brls::Box {
public:
    explicit TimelineView(const Config& cfg);
private:
    void load();
    void showMessage(const std::string& msg);
    void showTweets(const std::vector<Tweet>& tweets);
    brls::Box* list;
    XClient client;
};
