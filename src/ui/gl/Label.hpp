#ifndef __UI_LABEL_HPP__
#define __UI_LABEL_HPP__

#include "lvgl.h"

#include "Object.hpp"

#include <cstdio>

namespace ui
{

struct Label : Object
{
    template <typename... Ts>
    void setText(const char* fmt, Ts... ts)
    {
        char buff[64];
        snprintf(buff, 64, fmt, ts...);
        lv_label_set_text_fmt(this, "%s", buff);
    }
    char* getText()
    {
        return lv_label_get_text(this);
    }
};

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