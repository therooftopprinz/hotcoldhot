#ifndef __UI_TEXTAREA_HPP__
#define __UI_TEXTAREA_HPP__

#include "lvgl.h"

#include "Keyboard.hpp"
#include "Object.hpp"

namespace ui
{

struct Keyboard;

struct TextAreaData
{
    Keyboard* kb = nullptr;
};

struct TextArea : Object {};

class TextAreaBuilder
{
public:
    TextAreaBuilder() = delete;
    TextAreaBuilder(lv_obj_t* parent, bool, Keyboard*);
    TextArea* handle();

private:
    static void on_event(lv_event_t* e);
    TextArea* element = nullptr;
};


} // namespace ui

#endif // __UI_TEXTAREA_HPP__