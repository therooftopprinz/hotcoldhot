#include "GlobalStyles.hpp"

namespace ui
{

lv_style_t style_container;

void initGlobalStyles()
{
    lv_style_init(&style_container);
    lv_style_set_pad_all(&style_container, 0);
    lv_style_set_radius(&style_container, 0);
    lv_style_set_layout(&style_container, LV_LAYOUT_FLEX);
    lv_style_set_border_width(&style_container, 0);
}

} // namespace ui