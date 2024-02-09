#ifndef __UI_BUTTON_HPP__
#define __UI_BUTTON_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct Button : Object {};

class ButtonBuilder
{
public:
    ButtonBuilder() = delete;
    ButtonBuilder(lv_obj_t* parent);
    ButtonBuilder(lv_obj_t* parent, lv_event_cb_t cb,  void* user_data = nullptr, lv_event_code_t = LV_EVENT_CLICKED);
    Button* handle();

private:
    Button* element = nullptr;
};

} // namespace ui

#endif // __UI_BUTTON_HPP__