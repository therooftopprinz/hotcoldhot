#ifndef __UI_BUTTON_HPP__
#define __UI_BUTTON_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct ButtonData
{
    void* user_data = nullptr;
    lv_event_cb_t on_click = nullptr;
};

struct Button : Object {};


class ButtonBuilder
{
public:
    ButtonBuilder() = delete;
    ButtonBuilder(lv_obj_t* parent);
    Button* handle();

private:
    static void on_event(lv_event_t* e);

    Button* element = nullptr;
};

} // namespace ui

#endif // __UI_BUTTON_HPP__