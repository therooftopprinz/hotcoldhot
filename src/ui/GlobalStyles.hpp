#ifndef __UI_GLOBAL_STYLES_HPP__
#define __UI_GLOBAL_STYLES_HPP__

#include "lvgl.h"

namespace ui
{

extern lv_style_t style_nopadding;
extern lv_style_t style_debug_border;

void initGlobalStyles();

} // namespace ui

#endif // __UI_GLOBAL_STYLES_HPP__