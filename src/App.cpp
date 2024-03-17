#include "App.hpp"

#include <thread>

App::App()
    : ui(*this)
{

}

App::~App()
{

}

void App::loop()
{
    ui.loop();
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

bool App::start(const program_t&)
{
    return false;
}

bool App::stop()
{
    return false;
}

IApp::status_t App::status()
{
    return {};
}


std::pair<uint32_t, std::vector<IApp::sample_t>>
    App::get_chart(uint32_t offset, uint32_t size, uint32_t resample) 
{
    return {};
}