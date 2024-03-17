#include "TextArea.hpp"
#include "lv_utils.hpp"

namespace ui
{

const char* TextArea::getText()
{
    return lv_textarea_get_text(this);
}

TextAreaBuilder::TextAreaBuilder(lv_obj_t* parent, unsigned width, unsigned height, Keyboard* kb, bool should_hide_keyboard)
    : element((TextArea*)lv_textarea_create(parent))
{
    auto data = new TextAreaData;
    data->kb = kb;
    data->should_hide_keyboard = should_hide_keyboard;
    element->setUserData(data);
    element->addEventHandler(onEvent, LV_EVENT_ALL);
    if (!height)
    {
        lv_textarea_set_one_line(element, true);
    }
    else
    {
        element->setHeight(height);
    }
    element->setWidth(width);

    element->clearFlag(LV_OBJ_FLAG_SCROLLABLE);
}

TextArea* TextAreaBuilder::handle()
{
    return element;
}

void TextAreaBuilder::onEvent(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto ta = (TextArea*) lv_event_get_target(e);
    auto ta_data = (TextAreaData*) ta->getUserData();

    if(code == LV_EVENT_FOCUSED) {
        ta_data->kb->setTextarea(ta);
        ta_data->kb->show();
    }
    else if(LV_EVENT_DEFOCUSED  == code
            || LV_EVENT_READY   == code
            || LV_EVENT_CANCEL  == code)
     {
        ta_data->kb->setTextarea(nullptr);
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
