#include <board_devices.hpp>

#include <unordered_map>

#include "lvgl.h"

struct board_devices_context_t
{
    constexpr double zero_c = 273.15;
    // ambient1 -> load -> peltier -> heat_sink -> ambient2
    // load defs
    constexpr double load_kg = 0.1728;
    constexpr double load_J_Kkg = 900;
    constexpr double load_heatcap_J_K = (load_kg*load_J_Kkg); // 0.1728 kg * 900 J/(kg.K); a 40x40x40mm Al block
              double load_heat_J = load_J_Kkg*zero_c*load_kg; // load heat at 0c
    constexpr double load_to_ambient_W_m2k = 1;
    constexpr double load_to_ambient_m2 = 0.0096; // (40*6 mm2)
    constexpr double load_to_peltier_W_m2k = 12.7; // Grizzly Kryonaut

    // hs defs
    constexpr double peltier_to_hs_W_m2k = 12.7; // Grizzly Kryonaut
    constexpr double hs_kg = 0.5; // Grizzly Kryonaut
    constexpr double hs_heatcap_J_K = (0.5*load_J_Kkg); // 0.5 kg * 900 J/(kg.K); a large Al block
              double hs_heat_J = load_J_Kkg*zero_c*load_kg; // load heat at 0c
    constexpr double hs_to_ambient_W_m2k = 64;
    constexpr double hs_to_ambient_m2 = 0.04; // (200*200 mm2)
    constexpr double hs_to_peltier_W_m2k = 12.7; // Grizzly Kryonaut

    // peltier defs
    // E = (0.95 - T*0.0015)*I + T*0.003
    // Peltier effect Qp: Heat transport from one side to the other. Described in this equation Qp = I * α * T
    // Heat backflow QRth: Heat flow from the hot side to the cold side. Described in this equation QRth = dT / Rth
    // Joule heating/losses QRv represent in the resistance Rv: Described in this equation QRv = I2 * Rv / 2.
    // Qc = Qp - QRth - QRv.

    double peltier_A(double T, double E, double f=1)
    {
        return (E - T*0.003lf/(0.95 - T*0.0015))*f;
    }

    constexpr double peltier_f = 1;

    double peltier_QRth(double T)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    {
        return T*70*0.0016
    }

    double peltier_QRv(double T, double E)
    {
        auto I = peltier_A(T,E,peltier_f);
        return E*I/2;
    }

    double peltier_Qp(double T, double E)
    {
        auto I = peltier_A(T,E,peltier_f);
        return I*7.78;
    }

    double peltier_Qc(double T, double E)
    {
        auto I = peltier_A(T,E,peltier_f);
        return peltier_Qp(T,E) - peltier_QRth(T) - peltier_QRv(T,E);
    }

    std::thread runner;
    std::atomic<int> duty = 0;
    std::atomic<float> sensor = 0;
};

std::unordered_map<void*, board_devices_context_t> bdmap;

static void run_loop(board_devices_context_t& c)
{
    while (true)
    {
        // map duty to voltage
        double voltage = (fabs(c.duty)/100)*12;
        if (voltage >= 12) voltage = 12;
        bool cooling = sign(voltage);
        if (cooling)
        {
            // double src =  
            // double dst = 
        }

    }
}

board_devices::board_devices()
{
    auto& ctx = board_devices_context_t[this];
    ctx.runner = std::thread([&ctx](){
        run_loop(ctx);
    });
}

void set_peltier_duty(int d)
{
    auto& ctx = board_devices_context_t[this];
    ctx.duty = d;
}

float get_sensor_temp()
{
    auto& ctx = board_devices_context_t[this];
    return ctx.sensor;
}
