#ifndef __UI_MENUSETTINGS_HPP__
#define __UI_MENUSETTINGS_HPP__

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

class UI;
class MenuSettings
{
public:
    MenuSettings(UI&,TabView* parent);
private:
    static void onClickLoad(lv_event_t*);
    static void onClickSave(lv_event_t*);

    UI& ui;
    TabView* view = nullptr;
    Object* root = nullptr;
    Object* ctrls = nullptr;
    Object* fields = nullptr;
    Keyboard* kp = nullptr;
};

} // namespace ui

#endif // __UI_MENUSETTINGS_HPP__