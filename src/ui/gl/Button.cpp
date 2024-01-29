#include "Button.hpp"

namespace ui
{

ButtonBuilder::ButtonBuilder(lv_obj_t* parent)
    : element((Button*) lv_btn_create(parent))
{
    element->set_user_data(new ButtonData{});
    element->add_event_handler(on_event, LV_EVENT_ALL);
}

Button* ButtonBuilder::handle()
{
    return element;
}

void ButtonBuilder::on_event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto element = (Button*) lv_event_get_target(e);
    auto element_data = (ButtonData*) element->get_user_data();

    if(LV_EVENT_CLICKED == code && element_data && element_data->on_click)
    {
        (element_data->on_click)(e);
        return;
    }
    else if (LV_EVENT_DELETE == code)
    {
        delete element_data;
    }
}

} // namespace ui
