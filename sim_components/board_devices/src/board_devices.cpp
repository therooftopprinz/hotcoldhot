#include <board_devices.hpp>

#include <unordered_map>
#include <chrono>
#include <thread>
#include <cmath>
#include <atomic>
#include <cstdio>
#include <config.h>
#include <configmap.hpp>

struct board_devices_context_t
{
    // heats
    double heat_sensor = 0;
    double heat_load = 0;
    double heat_peltier_A = 0;
    double heat_peltier_B = 0;
    double heat_sink = 0;

    // heat mass J/K
    static constexpr double htm_sensor =  10.00; // Aluminum
    static constexpr double htm_load   = 155.52; // Aluminum
    static constexpr double htm_pplate =   5.10; // Alumina Ceramic
    static constexpr double htm_sink   = 450.00; // Aluminum

    // heat flow index W/K
    static constexpr double hfi_ambient_load =   0.16;
    static constexpr double hfi_load_sensor  =   0.20;
    static constexpr double hfi_pplate       =  40.00;
    static constexpr double hfi_junction     =   1.92;
    static constexpr double hfi_sink_ambient = 160.00;

    float N = 1;
    int64_t TS = 1;
    static constexpr double tmp_ambient = 25;

    double tmp_sensor() const    {return heat_sensor/htm_sensor - 273.15;}
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

board_devices_context_t ctx;

static int64_t time_us()
{
    return ctx.TS*std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void board_devices::loop()
{
    return;
}

int64_t board_devices::time_us()
{
    return ::time_us();
}

void initialize(board_devices_context_t& c)
{
    c.last_time = time_us();
    c.heat_sensor    = (273.15 + c.tmp_ambient) * c.htm_sensor;
    c.heat_load      = (273.15 + c.tmp_ambient) * c.htm_load;
    c.heat_peltier_B = (273.15 + c.tmp_ambient) * c.htm_pplate;
    c.heat_peltier_A = (273.15 + c.tmp_ambient) * c.htm_pplate;
    c.heat_sink      = (273.15 + c.tmp_ambient) * c.htm_sink;
    c.temp = c.tmp_ambient;

    configmap cm;
    cm.load(FS_PREFIX "/config.cfg");
    auto& cfg = cm.get();

    c.TS  = cm.at_or("T_Scale","1").as<int64_t>();
}

static void run(board_devices_context_t& c)
{
    int64_t delta = 0;
    unsigned dump = 0;
    initialize(c);
    while (true)
    {
        auto now = time_us();
        auto dt = double(now - c.last_time)/(1000000);
        delta += (now - c.last_time);
        c.last_time = now;

        // Peltier
        auto dT = c.tmp_peltier_B() - c.tmp_peltier_A();
        dT *= copysign(1.f, c.duty);
        auto E = c.duty*12;
        auto I = (fabs(E) - (dT*0.003)/(0.95-dT*0.0015))*c.N;

        // Heat Transfers Powers W
        double htp_load_sensor  = c.hfi_load_sensor  * (c.tmp_load()      - c.tmp_sensor());
        double htp_ambient_load = c.hfi_ambient_load * (c.tmp_ambient     - c.tmp_load());
        double htp_load_plateA  = c.hfi_pplate       * (c.tmp_load()      - c.tmp_peltier_A());
        double htp_backflow_AB  = c.hfi_junction     * (c.tmp_peltier_A() - c.tmp_peltier_B());
        double htp_peltier_AB   = c.peltier_coef * I * copysign(1.f, E);
        double htp_loss_AB      = fabs(E)        * I;
        double htp_plateB_sink  = c.hfi_pplate       * (c.tmp_peltier_B() - c.tmp_sink());
        double htp_sink_ambient = c.hfi_sink_ambient * (c.tmp_sink()      - c.tmp_ambient);

        // Heat Transfer
        c.heat_sensor       += htp_load_sensor*dt;
        c.heat_load         += (htp_ambient_load - htp_load_plateA)*dt;
        c.heat_peltier_A    += (htp_load_plateA + htp_loss_AB*0.5 - htp_backflow_AB - htp_peltier_AB )*dt;
        c.heat_peltier_B    += (htp_peltier_AB + htp_backflow_AB + htp_loss_AB*0.5 - htp_plateB_sink)*dt;
        c.heat_sink         += (htp_plateB_sink - htp_sink_ambient)*dt;

        c.temp = c.tmp_sensor();
        constexpr unsigned DUMP_NTH = 50000;
        if (dump==DUMP_NTH)
        {
            auto ave_delta = double(delta)/DUMP_NTH;
            printf("%lf %.10lf DUMP %3.3lf %3.3lf %3.3lf %3.3lf %3.3lf\n",
                double(time_us())/1000000,
                ave_delta,
                c.tmp_sensor(),
                c.tmp_load(),
                c.tmp_peltier_A(),
                c.tmp_peltier_B(), 
                c.tmp_sink());
            delta = 0;
            dump = 0;
        }
        dump++;
    }
}

board_devices::board_devices()
{
    ctx.runner = std::thread([](){
        run(ctx);
    });
}

void board_devices::set_peltier_duty_100(float d)
{
    auto da = fabs(d);
    ctx.N = ceil(da/100);
    float dd = fmod(da,100);
    if (dd == 0 && da > 0)
    {
        dd = 100;
    }
    ctx.duty = -copysign(dd, d)/100;
}

float board_devices::get_sensor_temp()
{
    return ctx.temp;
}