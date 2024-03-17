#include "MessageBox.hpp"

namespace ui
{

const char* MessageBoxBuilder::BUTTONS_OK[2] = {"Ok", ""};

MessageBoxBuilder::MessageBoxBuilder(lv_group_t* group)
    : m_group(group)
{}
    
MessageBoxBuilder& MessageBoxBuilder::title(const char* title)
{
    m_title = title;
    return *this;
}

MessageBoxBuilder& MessageBoxBuilder::text(const char* text)
{
    m_text = text;
    return *this;
}

MessageBoxBuilder& MessageBoxBuilder::buttons(const char** buttons)
{
    m_buttons = buttons;
    return *this;
}

void MessageBoxBuilder::show()
{
    lv_obj_t * mbox = lv_msgbox_create(nullptr, m_title, m_text, m_buttons, false);
    lv_obj_add_event_cb(mbox, onEvent, LV_EVENT_ALL, nullptr);
    lv_obj_align(mbox, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * bg = lv_obj_get_parent(mbox);
    lv_obj_set_style_bg_opa(bg, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(bg, lv_palette_main(LV_PALETTE_GREY), 0);
}

void MessageBoxBuilder::onEvent(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* msgbox = lv_event_get_current_target(e);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        const char* txt = lv_msgbox_get_active_btn_text(msgbox);
        if(txt)
        {
            lv_msgbox_close(msgbox);
        }
    }
}

} // namespace ui
