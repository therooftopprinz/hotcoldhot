#include "TextArea.hpp"
#include "lv_utils.hpp"

namespace ui
{

TextAreaBuilder::TextAreaBuilder(lv_obj_t* parent, unsigned width, unsigned height, Keyboard* kb, bool should_hide_keyboard)
    : element((TextArea*)lv_textarea_create(parent))
{
    auto data = new TextAreaData;
    data->kb = kb;
    data->should_hide_keyboard = should_hide_keyboard;
    element->set_user_data(data);
    element->add_event_handler(on_event, LV_EVENT_ALL);
    if (!height)
    {
        lv_textarea_set_one_line(element, true);
    }
    else
    {
        element->set_height(height);
    }
    element->set_width(width);
}

TextArea* TextAreaBuilder::handle()
{
    return element;
}

void TextAreaBuilder::on_event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto ta = (TextArea*) lv_event_get_target(e);
    auto ta_data = (TextAreaData*) ta->get_user_data();

    if(code == LV_EVENT_FOCUSED) {
        ta_data->kb->set_textarea(ta);
        ta_data->kb->show();
    }
    else if(LV_EVENT_DEFOCUSED  == code
            || LV_EVENT_READY   == code
            || LV_EVENT_CANCEL  == code)
     {
        ta_data->kb->set_textarea(nullptr);
        if (ta_data->should_hide_keyboard)
        {
            ta_data->kb->hide();
        }
    }
    else if (LV_EVENT_DELETE == code)
    {
        delete ta_data;
    }
}

} // namespace ui
