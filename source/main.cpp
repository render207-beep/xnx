#include <borealis.hpp>
#include "config.hpp"
#include "ui/timeline_view.hpp"

int main(int argc, char* argv[]) {
    brls::Logger::setLogLevel(brls::LogLevel::LOG_INFO);

    if (!brls::Application::init()) return EXIT_FAILURE;
    brls::Application::createWindow("X NX");

    Config cfg = Config::load("sdmc:/switch/xnx/config.json");

    auto* frame = new brls::AppletFrame(new TimelineView(cfg));
    frame->setTitle("X NX - Inicio");
    brls::Application::pushActivity(new brls::Activity(frame));

    while (brls::Application::mainLoop()) {}
    return EXIT_SUCCESS;
}
