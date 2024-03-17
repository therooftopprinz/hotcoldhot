#ifndef __UI_MESSAGEBOX_HPP__
#define __UI_MESSAGEBOX_HPP__

#include "lvgl.h"

namespace ui
{

class MessageBoxBuilder
{
public:
    MessageBoxBuilder(lv_group_t* group);
    MessageBoxBuilder& title(const char* title);
    MessageBoxBuilder& text(const char* text);
    MessageBoxBuilder& buttons(const char** buttons);
    void show();

    static const char* BUTTONS_OK[2];

private:
    static void onEvent(lv_event_t* e);

    lv_group_t* m_group = nullptr;
    const char* m_title = nullptr;
    const char* m_text = nullptr;
    const char** m_buttons = nullptr;
    void (*m_callback)(lv_obj_t* msgbox, const char* txt, bool& should_close) = nullptr;
};


} // namespace ui

#endif // __UI_MESSAGEBOX_HPP__
