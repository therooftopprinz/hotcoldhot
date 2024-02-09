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

class MenuProgram;

class ProgramControlBuilder
{
public:
    ProgramControlBuilder() = delete;
    ProgramControlBuilder(MenuProgram*);

private:
    static void on_event_append(lv_event_t* e);
    static void on_event_play(lv_event_t* e);
    static void on_event_stop(lv_event_t* e);

    MenuProgram* menu_program = nullptr;
    Object* parent = nullptr; 
    Rect* element = nullptr;
};

class ProgramListEntryBuilder
{
public:
    ProgramListEntryBuilder() = delete;
    ProgramListEntryBuilder(MenuProgram*,Object*);
    static void on_event_drag(lv_event_t* e);
    static void on_event_up(lv_event_t* e);
    static void on_event_down(lv_event_t* e);
    static void on_event_delete(lv_event_t* e);
    static void on_event_play(lv_event_t* e);

private:
    MenuProgram* menu_program = nullptr;
    Object* parent = nullptr; 
    Rect* element = nullptr;
};

class ProgramListBuilder
{
public:
    ProgramListBuilder() = delete;
    ProgramListBuilder(MenuProgram*);

private:
    MenuProgram* menu_program = nullptr;
    Object* parent = nullptr; 
    Rect* element = nullptr;
};

class MenuProgram
{
public:
    void new_row();
    MenuProgram(TabView* parent);
    Tab* get_tab();
    void on_event_play_pause();
    void on_event_lock_change(bool is_locked);
    void on_event_stop();
    void on_event_append();
    void on_event_sort(lv_event_t* e);
    void on_event_up(lv_event_t* e);
    void on_event_down(lv_event_t* e);
    void on_event_delete(lv_event_t* e);
    void on_event_play(lv_event_t* e);

private:
    TabView* view = nullptr;
    Tab* tab = nullptr;
};

} // namespace ui

#endif // __UI_MENUPROGRAM_HPP__