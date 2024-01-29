#include "TextArea.hpp"
#include "lv_utils.hpp"

namespace ui
{

TextAreaBuilder::TextAreaBuilder(lv_obj_t* parent, Keyboard* kb)
    : element((TextArea*)lv_textarea_create(parent))
{
    auto data = new TextAreaData;
    data->kb = kb;
    element->set_user_data(data);
    element->add_event_handler(on_event, LV_EVENT_ALL);
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
        ta_data->kb->hide();
    }
    else if (LV_EVENT_DELETE == code)
    {
        delete ta_data;
    }
}

} // namespace ui
