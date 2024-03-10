#include <memory>
#include "App.hpp"

std::unique_ptr<App> g_app;

extern "C" void setup()
{
    g_app = std::make_unique<App>();
}

extern "C" void loop()
{
    g_app->loop();
}
