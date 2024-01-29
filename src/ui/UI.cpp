#include <config.h>

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
    lv_init();
    lv_port_disp_init(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HEIGHT);
    lv_port_indev_init();

    initGlobalStyles();
    initInput();
    initUI();
}

void UI::run()
{
    while (lv_get_quit() == LV_QUIT_NONE)
    {
        lv_task_handler();
    }
}

UI::~UI()
{
    lv_port_disp_deinit();
    lv_port_indev_deinit();
    lv_deinit();
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
    menu = TabViewBuilder(lv_scr_act(), LV_DIR_BOTTOM, LV_DPI_DEF / 4).handle();

    g_keyboard = KeyboardBuilder(lv_scr_act()).handle();
    g_keyboard->hide();

    g_keypad = KeyboardBuilder(lv_scr_act()).handle();
    g_keypad->set_mode(LV_KEYBOARD_MODE_NUMBER);
    g_keypad->hide();

    menu_program = std::make_unique<MenuProgram>(menu);

    MessageBoxBuilder(group)
        .title("Welcome!")
        .text("Please see help to documentation!")
        .buttons(MessageBoxBuilder::BUTTONS_OK)
        .show();
}

} // namespace ui
