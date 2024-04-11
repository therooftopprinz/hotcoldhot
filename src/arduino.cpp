#include <memory>
#include "App.hpp"

extern "C"
{

#include <display_manager.h>
#include <storage_manager.h>
#include <network_manager.h>

}

std::unique_ptr<App> g_app;

void setup()
{
    network_manager_init();
    storage_manager_init();
    display_manager_init();
    g_app = std::make_unique<App>();
}

void loop()
{
    g_app->loop();
}
