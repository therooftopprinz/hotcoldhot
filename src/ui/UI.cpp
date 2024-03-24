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

UI::UI(IApp& app)
    : app(app)
{
    lv_log_register_print_cb(_log_cb);

    LV_LOG_INFO("UI | Initializing UI...");

    initGlobalStyles();
    initInput();
    initUI();

}

void UI::loop()
{
    lv_task_handler();
    menuRun->onStatus(app.status());
}

UI::~UI()
{
}

void UI::initInput()
{
    group = lv_group_create();
    lv_group_set_default(group);

    lv_indev_t* cur_drv = lv_indev_get_next(nullptr);

    while (cur_drv)
    {
        [[maybe_unused]] auto type = cur_drv->driver->type;
        LV_LOG_INFO("UI | indev: %d", type);
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

    menu = TabViewBuilder(screen, LV_DIR_BOTTOM, CONFIG_SCREEN_LINE_HEIGHT - RPAD).handle();
    lv_obj_set_scrollbar_mode(menu, LV_SCROLLBAR_MODE_OFF);

    auto menu_cnt = menu->getContent<Object>();
    menu_cnt->setWidth(CONFIG_SCREEN_WIDTH);
    menu_cnt->setHeight(CONFIG_SCREEN_HEIGHT-CONFIG_SCREEN_LINE_HEIGHT - RPAD);
    menu_cnt->setStylePadAll(5);
    menu_cnt->clearFlag(LV_OBJ_FLAG_SCROLLABLE);
    // lv_obj_set_style_border_width(menu_cnt, 1, LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(menu_cnt, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);

    menuRun = std::make_unique<MenuRun>(*this, menu);
    menuProgram = std::make_unique<MenuProgram>(*this, menu);
    menu->addTab("Settings");
    menu->addTab("Help");

    MessageBoxBuilder(group)
        .title("Welcome!")
        .text("Please see help to documentation!")
        .buttons(MessageBoxBuilder::BUTTONS_OK)
        .show();
}

std::pair<bool, IApp::program_t> UI::validate()
{
    return menuProgram->validate();
}

bool UI::start(const IApp::program_t& program)
{
    return app.start(program);
}

void UI::stop()
{
    app.stop();
}

lv_group_t* UI::getGroup()
{
    return group;
}

} // namespace ui
