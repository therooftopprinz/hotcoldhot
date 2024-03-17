#ifndef __BOARD_DEVICES_HPP__
#define __BOARD_DEVICES_HPP__

class board_devices
{
public:
    board_devices();
    void set_peltier_duty(int);
    float get_sensor_temp();
private:
};

#endif // __BOARD_DEVICE_MANAGER_HPP__