#ifndef __APP_HPP__
#define __APP_HPP__

#include "ui/UI.hpp"
#include "IApp.hpp"

#include <board_devices.hpp>

#include <thread>

class App : public IApp
{
public:
    App();
    ~App();
    void loop();
    bool start(const program_t&) override;
    bool stop() override;
    status_t status() override;
    SeriesView<short>& getChartTarget() override;
    SeriesView<short>& getChartActual() override;
private:
    void loadCfg();
    void check(int64_t);
    void setupTarget();
    bool nextTarget();
    void updateTarget(double);
    void calculateTLeft();

    SeriesView<short> serTarget;
    SeriesView<short> serActual;

    board_devices dev;
    ui::UI ui;

    int64_t lastPIDSample;

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
    std::optional<int64_t> targetEndTimeS = 0;
    std::optional<int64_t> targetStartTimeS = 0;
    double crossingTemp = 0;
    bool crossFromBelow = false;

    int64_t lastChartSample;

    float lastTemp = -273.15;

    int64_t nowUS;

    // tleft
    unsigned tLeftTarget = 0;
    std::optional<unsigned> tLeftRep;
    int64_t tLeft = 0;

    // PID
    double pwm = 0;
    double pidI = 0;
    double pidD = 0;
    double kPidP;
    double kPidI;
    double kPidD;
    double kPidTau = 0;
    double kPidCut = 100;
    double kPidLoopRate;
    double kPidS = 1;
    std::optional<double> oldErr;
    unsigned printPID = 0;
};

#endif // __APP_HPP__