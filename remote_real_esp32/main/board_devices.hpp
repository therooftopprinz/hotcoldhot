#ifndef __BOARD_DEVICES_HPP__
#define __BOARD_DEVICES_HPP__

#include <cstdint>

class board_devices
{
public:
    board_devices();
    int64_t time_us();
    void set_peltier_duty_100(float);
    float get_sensor_temp();
    void loop();
private:
};

#endif // __BOARD_DEVICE_MANAGER_HPP__