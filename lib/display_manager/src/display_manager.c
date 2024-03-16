#include <esp32_smartdisplay.h>

#include <display_manager.h>

void display_manager_init()
{
    smartdisplay_init();
    auto disp = lv_disp_get_default();
    // lv_disp_set_rotation(disp, LV_DISP_ROT_90);
    lv_disp_set_rotation(disp, LV_DISP_ROT_270);
}

void display_manager_deinit()
{}
