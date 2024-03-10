#ifndef __UI_MENUPROGRAM_HPP__
#define __UI_MENUPROGRAM_HPP__

#include <cstdio>

#include "lvgl.h"

#include "config.h"

#include <ui/UI.hpp>

#include <ui/gl/Button.hpp>
#include <ui/gl/Label.hpp>
#include <ui/gl/TextArea.hpp>
#include <ui/gl/Keyboard.hpp>
#include <ui/gl/TabView.hpp>
#include <ui/gl/Rect.hpp>

#include <ui/GlobalStyles.hpp>

namespace ui
{

class MenuProgram
{
public:
    void new_row();
    MenuProgram(TabView* parent);

private:

    TabView* view = nullptr;
    Object* root = nullptr;
};

} // namespace ui

#endif // __UI_MENUPROGRAM_HPP__