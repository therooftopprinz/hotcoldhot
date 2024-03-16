#include <memory>
#include "App.hpp"

std::unique_ptr<App> g_app;

void setup()
{
    g_app = std::make_unique<App>();
}

void loop()
{
    g_app->loop();
}
