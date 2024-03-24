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
    void check();
    void setupTarget();
    bool nextTarget();
    void updateTarget(double);

    board_devices dev;
    ui::UI ui;

    int64_t lastTime;

    static const char* states[5];

    enum state_e {
            E_STATE_IDLE,
            E_STATE_WAIT_TEMP,
            E_STATE_WAIT_TIME,
            E_STATE_PAUSED,
            E_STATE_ERROR
        };

    state_e state = E_STATE_IDLE;

    program_t program;
    unsigned target = 0;
    std::optional<unsigned> rep = 0;
    std::optional<int64_t> targetEndTime_s = 0;
    double crossingTemp = 0;
    bool crossFromBelow = false;

    // PID
    double pwm = 0;
    double pidI = 0;
};

#endif // __APP_HPP__