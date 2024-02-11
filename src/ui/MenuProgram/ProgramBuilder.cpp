#include "ProgramBuilder.hpp"

namespace ui
{

ProgramControlBuilder::ProgramControlBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->root;
    auto element = RectBuilder(parent).handle();
    menu_program->programControl = element;

    element->set_style_bg_color(lv_color_make(255, 200, 200)); 

    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);

    element->set_flex_flow(LV_FLEX_FLOW_ROW);
    element->set_flex_align(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    element->set_style_pad_column(2);
    element->set_style_pad_row(0);

    ButtonBuilder(element, LV_SYMBOL_PLUS, &MenuProgram::onEventAppend, menu_program).handle();
    ButtonBuilder(element, LV_SYMBOL_PLAY, &MenuProgram::onEventPlay, menu_program).handle();
    ButtonBuilder(element, LV_SYMBOL_STOP, &MenuProgram::onEventStop, menu_program).handle();
}

ProgramListBuilder::ProgramListBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->root;
    auto element = RectBuilder(parent).handle();
    menu_program->programList = element;

    element->set_style_bg_color(lv_color_make(200, 255, 200));

    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_HEIGHT - 2*CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);

    element->set_layout(LV_LAYOUT_FLEX);
    element->set_flex_flow(LV_FLEX_FLOW_COLUMN);

    element->set_style_pad_column(0);
    element->set_style_pad_row(0);
}

ProgramListEntryBuilder::ProgramListEntryBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->programList;
    auto element = RectBuilder(parent).handle();

    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    
    element->set_flex_flow(LV_FLEX_FLOW_ROW);
    element->set_flex_align(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    element->set_style_pad_column(2);
    element->set_style_pad_row(0);

    ButtonBuilder(element, LV_SYMBOL_LIST,  &MenuProgram::onEventInsertUp, menu_program).handle();
    ButtonBuilder(element, LV_SYMBOL_UP,    &MenuProgram::onEventInsertDown, menu_program).handle();
    ButtonBuilder(element, LV_SYMBOL_DOWN,  nullptr, menu_program).handle();
    ButtonBuilder(element, LV_SYMBOL_TRASH,  &MenuProgram::onEventDelete, menu_program).handle();

    auto temp_input = TextAreaBuilder(element, true, g_keypad).handle();
    temp_input->set_width(50);
    temp_input->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    LabelBuilder(element, "c ");

    auto time_input = TextAreaBuilder(element, true, g_keypad).handle();
    time_input->set_width(50);
    time_input->set_height(CONFIG_SCREEN_LINE_HEIGHT);

    LabelBuilder(element, "s ");

    ButtonBuilder(element, LV_SYMBOL_PLAY, nullptr, menu_program).handle();
   
}



} // namespace ui
