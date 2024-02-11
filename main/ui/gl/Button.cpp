#include "Button.hpp"
#include "Label.hpp"

namespace ui
{

ButtonBuilder::ButtonBuilder(lv_obj_t* parent)
    : element((Button*) lv_btn_create(parent))
{}

ButtonBuilder::ButtonBuilder(lv_obj_t* parent, const char* label, lv_event_cb_t cb, void* user_data, lv_event_code_t code)
    : element((Button*) lv_btn_create(parent))
{
    element->add_event_handler(cb, code, user_data);
    if (label)
    {
        LabelBuilder(element, label);
    }
}

Button* ButtonBuilder::handle()
{
    return element;
}

} // namespace ui
