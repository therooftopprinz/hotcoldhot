#include <board_devices.hpp>

#include <unordered_map>
#include <chrono>
#include <thread>
#include <cmath>
#include <atomic>
#include <chrono>

#include "lvgl.h"

struct board_devices_context_t
{
    // heats
    double heat_load = 0;
    double heat_peltier_A = 0;
    double heat_peltier_B = 0;
    double heat_sink = 0;

    // heat mass J/K
    static constexpr double htm_load   = 155.52; // Aluminum
    static constexpr double htm_pplate =   5.10; // Alumina Ceramic
    static constexpr double htm_sink   = 450.00; // Aluminum

    // heat flow index W/K
    static constexpr double hfi_ambient_load =   0.16;
    static constexpr double hfi_pplate       =  40.00;
    static constexpr double hfi_junction     =   1.92;
    static constexpr double hfi_sink_ambient = 160.00;

    static constexpr int N = 4;
    static constexpr double tmp_ambient = 25;

    double tmp_load() const      {return heat_load/htm_load - 273.15;}
    double tmp_peltier_A() const {return heat_peltier_A/htm_pplate - 273.15;}
    double tmp_peltier_B() const {return heat_peltier_B/htm_pplate - 273.15;}
    double tmp_sink() const      {return heat_sink/htm_sink - 273.15;}


    // Peltier
    double E = 0;
    double peltier_coef = 12.5;

    int64_t last_time;

    std::thread runner;
    std::atomic<float> duty = 0;
    std::atomic<float> temp = 0;
};

std::unordered_map<void*, board_devices_context_t> devmap;

static int64_t time_us()
{
    return 10*std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

int64_t board_devices::time_us()
{
    return ::time_us();
}

void initialize(board_devices_context_t& c)
{
    c.last_time = time_us();
    c.heat_load      = (273.15 + c.tmp_ambient) * c.htm_load;
    c.heat_peltier_B = (273.15 + c.tmp_ambient) * c.htm_pplate;
    c.heat_peltier_A = (273.15 + c.tmp_ambient) * c.htm_pplate;
    c.heat_sink      = (273.15 + c.tmp_ambient) * c.htm_sink;
    c.temp = c.tmp_ambient;
}

static void run(board_devices_context_t& c)
{
    initialize(c);
    uint64_t dumper = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        auto now = time_us();
        auto dt = double(now - c.last_time)/(1000000);
        c.last_time = now;

        // Peltier
        auto dT = c.tmp_peltier_B() - c.tmp_peltier_A();
        dT *= copysign(1.f, c.duty);
        auto E = c.duty*12;
        auto I = (fabs(E) - (dT*0.003)/(0.95-dT*0.0015))*c.N;

        // Heat Transfers Powers W
        double htp_ambient_load = c.hfi_ambient_load * (c.tmp_ambient     - c.tmp_load());
        double htp_load_plateA  = c.hfi_pplate       * (c.tmp_load()      - c.tmp_peltier_A());
        double htp_backflow_AB  = c.hfi_junction     * (c.tmp_peltier_A() - c.tmp_peltier_B());
        double htp_peltier_AB   = c.peltier_coef * I * copysign(1.f, E);
        double htp_loss_AB      = fabs(E)        * I;
        double htp_plateB_sink  = c.hfi_pplate       * (c.tmp_peltier_B() - c.tmp_sink());
        double htp_sink_ambient = c.hfi_sink_ambient * (c.tmp_sink()      - c.tmp_ambient);

        // Heat Transfer
        c.heat_load         += (htp_ambient_load - htp_load_plateA)*dt;
        c.heat_peltier_A    += (htp_load_plateA + htp_loss_AB*0.5 - htp_backflow_AB - htp_peltier_AB )*dt;
        c.heat_peltier_B    += (htp_peltier_AB + htp_backflow_AB + htp_loss_AB*0.5 - htp_plateB_sink)*dt;
        c.heat_sink         += (htp_plateB_sink - htp_sink_ambient)*dt;

        c.temp = c.tmp_load();
        if (0 == dumper%10000)
        {
            // LV_LOG_USER("DEV %.3lf %.3lf %.3lf %.3lf",
            //     c.tmp_load(),
            //     c.tmp_peltier_A(),
            //     c.tmp_peltier_B(),
            //     c.tmp_sink());
        }
        dumper++;
    }
}

board_devices::board_devices()
{
    auto& ctx = devmap[this];
    ctx.runner = std::thread([&ctx](){
        run(ctx);
    });
}

void board_devices::set_peltier_duty_100(float d)
{
    auto& ctx = devmap[this];
    ctx.duty = d/100;
}

float board_devices::get_sensor_temp()
{
    auto& ctx = devmap[this];
    return ctx.temp;
}