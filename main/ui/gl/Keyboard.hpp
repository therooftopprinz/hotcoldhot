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
    void set_mode(lv_keyboard_mode_t);
    void set_textarea(TextArea*);
};

class KeyboardBuilder
{
public:
    KeyboardBuilder(lv_obj_t*);
    Keyboard* handle();
private:
    Keyboard* element;
};

extern Keyboard* g_keyboard;
extern Keyboard* g_keypad;

} // namespace ui

#endif // __UI_MESSAGEBOX_HPP__*