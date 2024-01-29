#include "Label.hpp"

namespace ui
{

LabelBuilder::LabelBuilder(lv_obj_t* parent, const char* text)
    : element((Label*) lv_label_create(parent))
{
    lv_label_set_text(element, text);
}

Label* LabelBuilder::handle()
{
    return element;
}

} // namespace ui
