#ifndef __UI_UI_HPP__
#define __UI_UI_HPP__

#include <list>
#include <memory>
#include <cstdio>

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include <ui/gl/Keyboard.hpp>
#include <ui/gl/TabView.hpp>
#include <ui/gl/MessageBox.hpp>

#include "MenuProgram/MenuProgram.hpp"
#include "GlobalStyles.hpp"

namespace ui
{

class UI
{
public:
    UI();
    ~UI();
    void run();
private:
    void initStyles();
    void initInput();
    void initUI();

    lv_group_t* group = nullptr;
    TabView* menu = nullptr;
    std::unique_ptr<MenuProgram> menu_program;
};

} // namespace ui

#endif // __UI_UI_HPP__