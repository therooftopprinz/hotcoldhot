#ifndef __UI_MENUPROGRAM_HPP__
#define __UI_MENUPROGRAM_HPP__

#include <cstdio>

#include "lvgl.h"

#include "config.h"

#include <ui/UI.hpp>
#include <ui/MenuProgram/ProgramBuilder.hpp>

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

    static void onEventAppend(lv_event_t* e);
    static void onEventInsertUp(lv_event_t* e);
    static void onEventInsertDown(lv_event_t* e);
    static void onEventDelete(lv_event_t* e);
    static void onEventPlay(lv_event_t* e);
    static void onEventStop(lv_event_t* e);
    static void onEventLock(lv_event_t* e);

private:
    friend class ProgramControlBuilder;
    friend class ProgramListBuilder;
    friend class ProgramListEntryBuilder;

    TabView* view = nullptr;
    Object* root = nullptr;
    Rect* programControl = nullptr;
    Rect* programList = nullptr;
};

} // namespace ui

#endif // __UI_MENUPROGRAM_HPP__