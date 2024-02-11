#include "Keyboard.hpp"

namespace ui
{

void Keyboard::set_mode(lv_keyboard_mode_t mode)
{
    lv_keyboard_set_mode(this, mode);
}

void Keyboard::set_textarea(TextArea* ta)
{
    lv_keyboard_set_textarea(this, ta);
}

KeyboardBuilder::KeyboardBuilder(lv_obj_t* parent)
    : element((Keyboard*)lv_keyboard_create(parent))
{}

Keyboard* KeyboardBuilder::handle()
{
    return element;
}

Keyboard* g_keyboard = nullptr;
Keyboard* g_keypad = nullptr;

} // namespace ui