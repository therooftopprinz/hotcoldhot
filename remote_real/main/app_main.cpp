#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdio>
#include <thread>
#include <sstream>
#include <chrono>
#include "Udp.hpp"

#include "board_devices.hpp"

extern "C" void network_manager_init();

struct field_t
{
    enum {
        TAG_PWM,
        TAG_TEMP,
        TAG_TIME,
        TAG_TPWM = 'P'};
    uint8_t tag;
    int64_t value;
};

static int64_t now_us()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}


extern "C" void app_main()
{
    network_manager_init();
    board_devices dev{};
    printf("app_main loop");

    bfc::UdpSocket s;
    s.bind(bfc::toIp4Port("0.0.0.0:11111"));
    uint8_t b[1024];
    bfc::BufferView bv(b, sizeof(b));
    while (1)
    {
        bfc::Ip4Port from;
        auto rv = s.recvfrom(bv, from);
        if (rv<=0)
        {
            continue;
        }
        auto& f = *(field_t*) bv.data();
        auto nowlf = double(now_us())/1000000;
        if (f.tag == field_t::TAG_PWM)
        {
            float pwm = float(f.value)/100;
            printf("%lf SET PWM %f\n", nowlf, pwm);
            dev.set_peltier_duty_100(pwm);
        }
        else if (f.tag == field_t::TAG_TEMP)
        {
            f.value = dev.get_sensor_temp()*100;
            printf("%lf GET TMP %f\n", nowlf, float(f.value)/100);
        }
        else if (f.tag == field_t::TAG_TPWM)
        {
            auto sdata = (char*) (b+1);
            b[rv] = 0;
            std::stringstream ss{sdata};
            float pwm;
            ss >> pwm;
            printf("%lf SET PWM %f\n", nowlf, pwm);
            dev.set_peltier_duty_100(pwm);
        }
        else if (f.tag == field_t::TAG_TIME)
        {
            f.value = dev.time_us();
            printf("%lf GET TME %f\n", nowlf, float(f.value)/100);
        }
        s.sendto(bfc::BufferView(b, rv), from);
    }

}