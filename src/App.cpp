#include "App.hpp"

#include <thread>
#include <cmath>

#include <configmap.hpp>
#include "config.h"

const char* App::states[5] = {
        "IDLE",
        "WAIT_TEMP",
        "WAIT_TIME",
        "PAUSED",
        "ERROR"
    };

App::App()
    : serTarget(FS_PREFIX "/target.ser", 60)
    , serActual(FS_PREFIX "/actual.ser", 60)
    , ui(*this)
{
    LV_LOG_USER("Initializing App...");
    nowUS = dev.time_us();
    lastPIDSample = nowUS;
    lastChartSample = nowUS;
    loadCfg();
}

App::~App()
{
}

void App::loadCfg()
{
    configmap cm;
    cm.load(FS_PREFIX "/config.cfg");
    auto& cfg = cm.get();

    for (auto& e : cfg)
    {
        LV_LOG_USER("config: %s = %s", e.first.c_str(), e.second.raw().c_str());
    }

    kPidP  = cm.at_or("PID_P","5").as<double>();
    kPidI  = cm.at_or("PID_I","0").as<double>();
    kPidD  = cm.at_or("PID_D","0").as<double>();
    kPidTau = cm.at_or("PID_U","1").as<double>();
    kPidLoopRate = cm.at_or("PID_Rate","0.2").as<double>();
}

void App::loop()
{
    dev.loop();
    nowUS = dev.time_us();
    auto now = nowUS;
    double dt = double(now - lastPIDSample)/1000000;
    if (dt >= kPidLoopRate)
    {
        lastPIDSample = now;
        updateTarget(dt);
    }

    check(now);

    ui.loop();

    if ((now - lastChartSample) >= (1000*1000))
    {
        lastChartSample = now;
        short targetX = program.tt_config[target].first*100;
        short currentX = lastTemp*100;
        if (E_STATE_IDLE == state)
        {
            serTarget.push(currentX);
        }
        else
        {
            serTarget.push(targetX);
        }

        serActual.push(currentX);
    }
}

void App::updateTarget(double dt)
{
    auto temp0 = lastTemp;
    lastTemp = dev.get_sensor_temp();
    auto temp = lastTemp;

    if (E_STATE_IDLE == state ||
        E_STATE_ERROR == state)
    {
        return;
    }

    double error = program.tt_config[target].first - temp;
    double error0 = oldErr.value_or(error);
    oldErr = error;
    double dedt = (error-error0)*dt;

    // p coef
    auto pidP = error*kPidP;

    // i coef
    pidI += 0.5*kPidI*dt*(error + error0);

    // dynamic integrator clamping
    double pidImax = 0;
    double pidImin = 0;
    if (kPidCut > pidP)
    {
        pidImax = kPidCut-pidP;
    }

    if (-kPidCut < pidP)
    {
        pidImin = -kPidCut-pidP;
    }

    if (pidI > pidImax) pidI = pidImax;
    if (pidI < pidImin) pidI = pidImin;

    // d coef
    pidD =   (2.f * kPidD * (temp - temp0) +  pidD * (2.f * kPidTau - dt))
            /(2.f * kPidTau + dt);

    auto pid = pidP + pidI + pidD;
    if (pid > kPidCut)  pid = kPidCut;
    if (pid < -kPidCut) pid = -kPidCut;

    pwm = pid;
   
    LV_LOG_USER("App | PID %lf %12.3lf %12.3lf %12.3lf %12.3lf %12.3lf %12.3lf",
        double(nowUS)/1000000, temp, error, pid, pidP, pidI, pidD);   

    printPID++;

    dev.set_peltier_duty_100(pwm);
}

bool App::start(const program_t& p)
{
    loadCfg();

    pidI = 0;
    pidD = 0;

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
    tLeftRep.reset();
    target = 0;
    pidI = 0;
    tLeftTarget = -1;
    calculateTLeft();
    setupTarget();

    return true;
}

bool App::stop()
{
    target = 0;
    state = E_STATE_IDLE;
    pwm = 0;
    rep.reset();
    targetEndTimeS.reset();
    tLeftRep.reset();
    dev.set_peltier_duty_100(0);
    return true;
}

void App::calculateTLeft()
{
    if (tLeftTarget != target || tLeftRep != rep)
    {
        tLeftTarget = target;
        tLeftRep = rep;
        tLeft = 0;
        auto xtarget = target;
        auto xrep = rep.value_or(1);
        while (true)
        {
            auto tt = program.tt_config[xtarget].second;
            tLeft += tt;
            LV_LOG_USER("App | status left: T%d-R%d tt=%d", xtarget+1, xrep, tt);
            if (3 == xtarget && xrep < program.rep_cnt)
            {
                xtarget = 1;
                xrep++;
            }
            else
            {
                xtarget++;
            }
            if (xtarget == 5)
            {
                break;
            }
        }
        LV_LOG_USER("App | status left: total=%ld", tLeft);
    }
}

IApp::status_t App::status()
{
    auto now_s = nowUS/1000000;

    IApp::status_t status{};
    status.state = states[state];

    status.currentActualT = lastTemp;

    calculateTLeft();

    if (E_STATE_IDLE != state)
    {
        status.currentTargetN = target;
        status.currentTargetT = program.tt_config[target].first;
        auto left = tLeft;
        if (targetStartTimeS)
        {
            left -= (now_s - *targetStartTimeS);
        }
        status.totalRemaining = left;
    }

    if (targetEndTimeS && *targetEndTimeS >= now_s)
    {
        status.currentRemaining = *targetEndTimeS - now_s;
    }

    if (target >= 1 && target<4)
    {
        status.repn = rep;
    }

    status.pwm = pwm;

    return status;
}

void App::setupTarget()
{
    if (target>=5 || target<0)
    {
        return;
    }

    auto targetTemp = program.tt_config[target].first;
    targetEndTimeS.reset();
    targetStartTimeS.reset();
    state = E_STATE_WAIT_TEMP;

    auto temp = lastTemp;

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

void App::check(int64_t now)
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
        auto temp = lastTemp;
        if (crossFromBelow)
        {
            if (temp >= (crossingTemp-2))
            {
                state = E_STATE_WAIT_TIME;
            }
        }
        else
        {
            if (temp <= (crossingTemp+2))
            {
                state = E_STATE_WAIT_TIME;
            }
        }
    }

    auto now_s = now/1000000;
    if (E_STATE_WAIT_TEMP == old_state && state == E_STATE_WAIT_TIME)
    {
        auto prog_s = program.tt_config[target].second;
        targetStartTimeS.emplace(now_s);
        targetEndTimeS.emplace(now_s+prog_s);
    }

    if (E_STATE_WAIT_TIME == state)
    {
        if (now_s >= *targetEndTimeS)
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
        target = 1;
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

SeriesView<short>& App::getChartTarget()
{
    return serTarget;
}

SeriesView<short>& App::getChartActual()
{
    return serActual;
}
