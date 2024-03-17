#ifndef __APP_HPP__
#define __APP_HPP__

#include "ui/UI.hpp"
#include "IApp.hpp"

#include <board_devices.hpp>

class App : public IApp
{
public:
    App();
    ~App();
    void loop();
    bool start(const program_t&) override;
    bool stop() override;
    status_t status() override;
    std::pair<uint32_t, std::vector<sample_t>> get_chart(uint32_t offset, uint32_t size, uint32_t resample) override;
private:
    ui::UI ui;

    program_t program;
    unsigned target = 0;

};

#endif // __APP_HPP__