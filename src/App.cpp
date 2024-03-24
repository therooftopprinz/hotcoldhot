#include "App.hpp"

#include <thread>
#include <cmath>

const char* App::states[5] = {
        "IDLE",
        "WAIT_TEMP",
        "WAIT_TIME",
        "PAUSED",
        "ERROR"
    };

App::App()
    : ui(*this)
{
    lastTime = dev.time_us();
    LV_LOG_USER("Initializing App...");
}

App::~App()
{}

void App::loop()
{
    check();
    auto now = dev.time_us();
    double dt = double(dev.time_us() - lastTime)/10000000;
    lastTime = now;
    updateTarget(dt);
    ui.loop();
}

void App::updateTarget(double dt)
{
    if (E_STATE_IDLE == state ||
        E_STATE_ERROR == state)
    {
        return;
    }

    auto temp = dev.get_sensor_temp();
    double error = program.tt_config[target].first - temp;

    // p coef
    auto p = -error*40;
    if (p > 12)  p = 12;
    if (p < -12) p = -12;

    // i coef
    pidI += (p*0.5);
    if (pidI > 12)  pidI = 12;
    if (pidI < -12) pidI = -12;

    auto pi = p + pidI;
    if (pi > 12)  pi = 12;
    if (pi < -12) pi = -12;

    pwm = (pi/12)*100;
    dev.set_peltier_duty_100(pwm);
}

bool App::start(const program_t& p)
{
    if (E_STATE_IDLE != state)
    {
        return false;
    }

    // @todo : validate temp range
    program = p;

    LV_LOG_USER("App | start rep: %u", program.rep_cnt);
    for (size_t i=0; i<5; i++)
    {
        LV_LOG_USER("App | temp[%lu]: %f", i, program.tt_config[i].first);
        LV_LOG_USER("App | time[%lu]: %d", i, program.tt_config[i].second);
    }

    dev.set_peltier_duty_100(0);
    rep.reset();
    target = 0;
    pidI = 0;
    setupTarget();

    return true;
}

bool App::stop()
{
    target = 0;
    state = E_STATE_IDLE;
    pwm = 0;
    rep.reset();
    targetEndTime_s.reset();
    dev.set_peltier_duty_100(0);
    return true;
}

IApp::status_t App::status()
{
    auto now_s = dev.time_us()/1000000;

    IApp::status_t status{};
    status.state = states[state];

    status.currentActualT = dev.get_sensor_temp();

    if (state != E_STATE_IDLE)
    {
        status.currentTargetN = target;
        status.currentTargetT = program.tt_config[target].first;
    }

    if (targetEndTime_s && *targetEndTime_s >= now_s)
    {
        status.currentRemaining = *targetEndTime_s - now_s;
    }

    status.repn = rep;
    status.pwm = pwm;

    return status;
}

std::pair<uint32_t, std::vector<IApp::sample_t>>
    App::get_chart(uint32_t offset, uint32_t size, uint32_t resample) 
{
    return {};
}

void App::setupTarget()
{
    if (target>=5 || target<0)
    {
        return;
    }

    auto targetTemp = program.tt_config[target].first;
    targetEndTime_s.reset();

    state = E_STATE_WAIT_TEMP;

    auto temp = dev.get_sensor_temp();

    // @todo : configurable diff
    if (temp < targetTemp)
    {
        crossFromBelow = true;
        crossingTemp = targetTemp - 0.5;
    }
    else
    {
        crossingTemp = targetTemp + 0.5;
        crossFromBelow = false;
    }
}

void App::check()
{
    auto old_state = state;
    auto old_target = target;

    if (E_STATE_IDLE == state ||
        E_STATE_PAUSED == state ||
        E_STATE_ERROR == state)
    {
        return;
    }

    if (E_STATE_WAIT_TEMP == state)
    {
        auto temp = dev.get_sensor_temp();
        if (crossFromBelow)
        {
            if (temp >= crossingTemp)
            {
                state = E_STATE_WAIT_TIME;
            }
        }
        else
        {
            if (temp <= crossingTemp)
            {
                state = E_STATE_WAIT_TIME;
            }
        }
    }

    auto now_s = dev.time_us()/1000000;
    if (E_STATE_WAIT_TEMP == old_state && state == E_STATE_WAIT_TIME)
    {
        auto prog_s = program.tt_config[target].second;
        targetEndTime_s.emplace(now_s+prog_s);
    }

    if (E_STATE_WAIT_TIME == state)
    {
        if (now_s >= *targetEndTime_s)
        {
            if (nextTarget())
            {
                setupTarget();
            }
            else
            {
                stop();
            }
        }
    }

    if (state != old_state)
    {
        LV_LOG_USER("App | state update : %d to %d", old_state, state);
    }
    if (target != old_target && E_STATE_IDLE != state)
    {
        LV_LOG_USER("App | target update : %d to %d", old_target, target);
        LV_LOG_USER("App | target temp: %f", program.tt_config[target].first);
        LV_LOG_USER("App | target time: %d", program.tt_config[target].second);
    }
}

bool App::nextTarget()
{
    if (target==0)
    {
        rep.emplace(1);
    }

    if (3 == target && *rep >= program.rep_cnt)
    {
        target = 4;
    }
    else if (3 == target)
    {
        target = 2;
        (*rep)++;
    }
    else
    {
        target++;
    }

    if (target == 5)
    {
        target == 0;
        return false;
    }

    return true;
}
