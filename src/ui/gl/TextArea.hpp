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
    bool should_hide_keyboard = true;
};

struct TextArea : Object
{
    const char* getText();
    void setText(const char*);
};

class TextAreaBuilder
{
public:
    TextAreaBuilder() = delete;
    TextAreaBuilder(lv_obj_t* parent, unsigned, unsigned, Keyboard*, bool=true);
    TextArea* handle();

private:
    static void onEvent(lv_event_t* e);
    TextArea* element = nullptr;
};


} // namespace ui

#endif // __UI_TEXTAREA_HPP__