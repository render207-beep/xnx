#include "ui/timeline_view.hpp"

TimelineView::TimelineView(const Config& cfg) : client(cfg) {
    setAxis(brls::Axis::COLUMN);
    setGrow(1.0f);

    auto* scroll = new brls::ScrollingFrame();
    scroll->setGrow(1.0f);
    list = new brls::Box(brls::Axis::COLUMN);
    list->setPadding(20);
    scroll->setContentView(list);
    addView(scroll);

    registerAction("Recargar", brls::ControllerButton::BUTTON_X,
                   [this](brls::View*) { load(); return true; });
    showMessage("Cargando...");
    load();
}

void TimelineView::showMessage(const std::string& msg) {
    list->clearViews();
    auto* l = new brls::Label();
    l->setText(msg);
    l->setFontSize(20);
    list->addView(l);
}

void TimelineView::showTweets(const std::vector<Tweet>& tweets) {
    list->clearViews();
    if (tweets.empty()) { showMessage("Sin resultados."); return; }
    for (auto& t : tweets) {
        auto* card = new brls::Box(brls::Axis::COLUMN);
        card->setMarginBottom(16);
        card->setPadding(12);
        card->setFocusable(true);

        auto* head = new brls::Label();
        head->setText(t.name + "  @" + t.handle);
        head->setFontSize(18);
        auto* body = new brls::Label();
        body->setText(t.text);
        body->setFontSize(20);

        card->addView(head);
        card->addView(body);
        list->addView(card);
    }
}

void TimelineView::load() {
    // Red en hilo aparte; la UI se toca solo desde brls::sync
    brls::async([this]() {
        TimelineResult r = client.homeTimeline();
        brls::sync([this, r]() {
            if (!r.error.empty()) showMessage("Error: " + r.error);
            else showTweets(r.tweets);
        });
    });
}
