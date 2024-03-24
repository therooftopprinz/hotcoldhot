#ifndef __UI_MENURUN_HPP__
#define __UI_MENURUN_HPP__

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

class MenuRun
{
public:
    MenuRun(UI&,TabView* parent);
    void onStatus(const IApp::status_t&);
private:
    static void onClickStart(lv_event_t*);
    static void onClickStop(lv_event_t*);

    UI& ui;

    TabView* view = nullptr;
    Object* root = nullptr;
    Object* chart = nullptr;

    Label* state   = nullptr;
    Label* targetT = nullptr;
    Label* targetN = nullptr;
    Label* leftH   = nullptr;
    Label* leftS   = nullptr;
    Label* mode    = nullptr;
    Label* pwm     = nullptr;
    Label* tleftH  = nullptr;
    Label* tleftS  = nullptr;
};

} // namespace ui

#endif // __UI_MENURUN_HPP__