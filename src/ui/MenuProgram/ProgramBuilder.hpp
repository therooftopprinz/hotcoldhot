#ifndef __UI_MENUPROGRAM_PROGRAMBUILDER_HPP__
#define __UI_MENUPROGRAM_PROGRAMBUILDER_HPP__

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
};

class ProgramListBuilder
{
public:
    ProgramListBuilder() = delete;
    ProgramListBuilder(MenuProgram*);
};

class ProgramListEntryBuilder
{
public:
    ProgramListEntryBuilder() = delete;
    ProgramListEntryBuilder(MenuProgram*);
    Rect* handle();
private:
    Rect* element;
};

} // namespace ui

#endif // __UI_MENUPROGRAM_PROGRAMBUILDER_HPP__