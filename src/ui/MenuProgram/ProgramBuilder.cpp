#include "ProgramBuilder.hpp"

namespace ui
{

ProgramControlBuilder::ProgramControlBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->tab;
    auto element = RectBuilder(nullptr).handle();
    menu_program->programControl = element;

    lv_obj_set_pos(element, 250, 250);
    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    element->set_style_pad_all(1, LV_STATE_DEFAULT);
    lv_obj_update_layout(element);

    auto button_append = ButtonBuilder(element, &MenuProgram::on_event_append, menu_program).handle();
    auto button_play   = ButtonBuilder(element, &MenuProgram::on_event_play, menu_program).handle();
    auto button_stop   = ButtonBuilder(element, &MenuProgram::on_event_stop, menu_program).handle();

    LabelBuilder(button_append, LV_SYMBOL_PLUS).handle();
    LabelBuilder(button_play, LV_SYMBOL_PLAY).handle();
    LabelBuilder(button_stop, LV_SYMBOL_STOP).handle();

    button_append->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
    button_play->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
    button_stop->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
}

ProgramListBuilder::ProgramListBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->tab;
    auto element = RectBuilder(parent).handle();
    menu_program->programList = element;

    lv_obj_set_pos(element, 0, CONFIG_SCREEN_LINE_HEIGHT/2);
    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_HEIGHT - 2*CONFIG_SCREEN_LINE_HEIGHT);

    lv_obj_update_layout(element);

    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    element->set_style_pad_all(1, LV_STATE_DEFAULT);
    element->set_layout(LV_LAYOUT_FLEX);
    element->set_flex_flow(LV_FLEX_FLOW_COLUMN);
}

ProgramListEntryBuilder::ProgramListEntryBuilder(MenuProgram* menu_program)
{
    auto parent = menu_program->programList;
    auto element = RectBuilder(parent).handle();

    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    element->set_style_pad_all(1, LV_STATE_DEFAULT);

    auto button_ins_up     = ButtonBuilder(element, nullptr, menu_program).handle();
    auto button_ins_down   = ButtonBuilder(element, nullptr, menu_program).handle();

    LabelBuilder(button_ins_up, LV_SYMBOL_UP).handle();
    LabelBuilder(button_ins_down, LV_SYMBOL_DOWN).handle();

}



} // namespace ui
