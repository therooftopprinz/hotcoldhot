#ifndef __UI_KEYPAD_HPP__
#define __UI_KEYPAD_HPP__

#include "lvgl.h"

#include "TextArea.hpp"
#include "Object.hpp"

namespace ui
{

struct TextArea;

struct Keyboard : Object
{
    void setMode(lv_keyboard_mode_t);
    void setTextarea(TextArea*);
};

class KeyboardBuilder
{
public:
    KeyboardBuilder(lv_obj_t*);
    Keyboard* handle();
private:
    Keyboard* element;
};

} // namespace ui

#endif // __UI_MESSAGEBOX_HPP__*