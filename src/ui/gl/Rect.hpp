#ifndef __UI_RECT_HPP__
#define __UI_RECT_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct Rect : Object {};

class RectBuilder
{
public:
    RectBuilder() = delete;
    RectBuilder(lv_obj_t* parent)
        : element((Rect*) lv_obj_create(parent))
    {}

    Rect* handle()
    {
        return element;
    }

private:
    Rect* element = nullptr;
};


} // namespace ui

#endif // __UI_BASE_HPP__