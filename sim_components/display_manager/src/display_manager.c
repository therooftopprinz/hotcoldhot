#include <display_manager.h>
#include <config.h>

#include "lv_port_disp.h"
#include "lv_port_indev.h"

void display_manager_init()
{
    lv_init();
    lv_port_disp_init(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HEIGHT);
    lv_port_indev_init();
}

void display_manager_deinit()
{
    lv_port_disp_deinit();
    lv_port_indev_deinit();
    // lv_deinit();
}
