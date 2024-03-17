#include <memory>
#include "App.hpp"

extern "C"
{

#include <display_manager.h>
#include <storage_manager.h>

}

std::unique_ptr<App> g_app;

void setup()
{
    storage_manager_init();
    display_manager_init();
    g_app = std::make_unique<App>();
}

void loop()
{
    g_app->loop();
}
