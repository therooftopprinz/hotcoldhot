#ifndef __UI_LABEL_HPP__
#define __UI_LABEL_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct Label : Object{};

class LabelBuilder
{
public:
    LabelBuilder() = delete;
    LabelBuilder(lv_obj_t* parent, const char* text);
    Label* handle();

private:
    static void on_event(lv_event_t* e) {}
    Label* element = nullptr;
};

} // namespace ui

#endif // __UI_TABVIEW_HPP__