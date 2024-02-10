#include "MenuProgram.hpp"

namespace ui
{

MenuProgram::MenuProgram(TabView* parent)
    : view(parent)
    , tab (parent->add_tab("Program"))
{
    tab->add_style(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    // tab->set_layout(LV_LAYOUT_FLEX);
    // tab->set_flex_flow(LV_FLEX_FLOW_COLUMN);
    // LV_LAYOUT_

    ProgramControlBuilder{this};
    // ProgramListBuilder{this};
}

void MenuProgram::on_event_append(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_current_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::on_event_append);

    if(LV_EVENT_CLICKED == code)
    {
        ProgramListEntryBuilder{this_};
    }
}

void MenuProgram::on_event_play(lv_event_t* e)
{}

void MenuProgram::on_event_stop(lv_event_t* e)
{}

void MenuProgram::on_event_lock_change(lv_event_t* e)
{}

} // namespace ui
