#ifndef __UI_UI_HPP__
#define __UI_UI_HPP__

#include <list>
#include <memory>
#include <cstdio>
#include <functional>

#include "lvgl.h"

#include <ui/gl/Keyboard.hpp>
#include <ui/gl/TabView.hpp>
#include <ui/gl/MessageBox.hpp>

#include <IApp.hpp>

#include "menu/MenuProgram.hpp"
#include "menu/MenuRun.hpp"
#include "GlobalStyles.hpp"

namespace ui
{

constexpr auto CPAD = 5;
constexpr auto RPAD = 5;

class MenuProgram;
class MenuRun;

class UI
{
public:
    UI(IApp&);
    ~UI();
    void loop();

    std::pair<bool,IApp::program_t> validate();
    bool start(const IApp::program_t&);
    void stop();
    lv_group_t* getGroup();

private:
    void initInput();
    void initUI();

    IApp& app;

    lv_group_t* group = nullptr;
    TabView* menu = nullptr;
    std::unique_ptr<MenuProgram> menuProgram;
    std::unique_ptr<MenuRun> menuRun;
};

} // namespace ui

#endif // __UI_UI_HPP__