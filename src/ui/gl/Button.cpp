#include "Button.hpp"
#include "Label.hpp"

namespace ui
{

Label* Button::getLabel()
{
    if (1 > getChildCnt())
    {
        return nullptr;
    }
    return (Label*) getChild(0);
}

ButtonBuilder::ButtonBuilder(lv_obj_t* parent)
    : element((Button*) lv_btn_create(parent))
{}

ButtonBuilder::ButtonBuilder(lv_obj_t* parent, const char* label, lv_event_cb_t cb, void* user_data, lv_event_code_t code)
    : element((Button*) lv_btn_create(parent))
{
    element->addEventHandler(cb, code, user_data);
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
