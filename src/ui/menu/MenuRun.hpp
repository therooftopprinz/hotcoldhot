#ifndef __UI_MENURUN_HPP__
#define __UI_MENURUN_HPP__

#include <cstdio>

#include "lvgl.h"

#include "config.h"

#include <IApp.hpp>
#include <ui/menu/MenuProgram.hpp>

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
class MenuProgram;

class MenuRun
{
public:
    MenuRun(UI&, IApp&, MenuProgram&, TabView* parent);
    void loop();
private:
    void updateStatus();
    bool updateChart();
    void updateZoom();
    static void onClickStart(lv_event_t*);
    static void onClickStop(lv_event_t*);
    static void onClickScrollLeft(lv_event_t*);
    static void onClickScrollRight(lv_event_t*);
    static void onClickScrollPlus(lv_event_t*);
    static void onClickScrollMinus(lv_event_t*);
    static void onClickScrollFirst(lv_event_t*);
    static void onClickScrollLast(lv_event_t*);

    UI& ui;
    IApp& app;
    MenuProgram& program;

    TabView* view = nullptr;
    Object* root = nullptr;
    lv_chart_series_t *serTarget  = nullptr;
    lv_chart_series_t *serCurrent = nullptr;
    Object* chart = nullptr;

    int scrollZoomLevel = 1;
    static constexpr unsigned scrollConst = 60;  

    Label* state   = nullptr;
    Label* targetT = nullptr;
    Label* targetN = nullptr;
    Label* leftH   = nullptr;
    Label* leftS   = nullptr;
    Object* mode   = nullptr;
    Label* pwm     = nullptr;
    Label* tleftH  = nullptr;
    Label* tleftS  = nullptr;
};

} // namespace ui

#endif // __UI_MENURUN_HPP__