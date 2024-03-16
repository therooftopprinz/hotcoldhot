#include <config.h>

extern "C"
{
#include <display_manager.h>
}

#include "UI.hpp"

namespace ui
{

lv_style_t g_style_debug_border;

void _log_cb(const char *buf)
{
    printf("%s", buf);
    fflush(stdout);
}

UI::UI()
{
    lv_log_register_print_cb(_log_cb);

    LV_LOG_INFO("Initializing UI...");

    display_manager_init();

    initGlobalStyles();
    initInput();
    initUI();

}

void UI::loop()
{
    lv_task_handler();
}

UI::~UI()
{
    display_manager_deinit();
}

void UI::initInput()
{
    group = lv_group_create();
    lv_group_set_default(group);

    lv_indev_t* cur_drv = lv_indev_get_next(nullptr);

    while (cur_drv)
    {
        auto type = cur_drv->driver->type;
        LV_LOG_INFO("indev: %d", type);
        if(cur_drv->driver->type == LV_INDEV_TYPE_KEYPAD)
        {
            lv_indev_set_group(cur_drv, group);
        }

        cur_drv = lv_indev_get_next(cur_drv);
    }
}

void UI::initUI()
{
    auto screen = (Object*) lv_scr_act();

    menu = TabViewBuilder(screen, LV_DIR_BOTTOM, CONFIG_SCREEN_LINE_HEIGHT).handle();
    lv_obj_set_scrollbar_mode(menu, LV_SCROLLBAR_MODE_OFF);

    menu_program = std::make_unique<MenuProgram>(menu);

    menu->add_tab("Status");
    menu->add_tab("Settings");
    menu->add_tab("Help");

    MessageBoxBuilder(group)
        .title("Welcome!")
        .text("Please see help to documentation!")
        .buttons(MessageBoxBuilder::BUTTONS_OK)
        .show();
}

} // namespace ui
