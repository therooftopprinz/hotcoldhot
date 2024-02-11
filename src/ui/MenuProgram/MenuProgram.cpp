#include "MenuProgram.hpp"

namespace ui
{

MenuProgram::MenuProgram(TabView* parent)
    : view(parent)
{
    auto tab = parent->add_tab("Program");
    tab->add_style(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*Create a container with grid*/
    static lv_coord_t col_dsc[] = {
                                    CONFIG_SCREEN_WIDTH,
                                    LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_HEIGHT - 2*CONFIG_SCREEN_LINE_HEIGHT,
                                    LV_GRID_TEMPLATE_LAST};

    root = (Object*) lv_obj_create(tab);
    root->set_width(CONFIG_SCREEN_WIDTH);
    root->set_height(CONFIG_SCREEN_HEIGHT - CONFIG_SCREEN_LINE_HEIGHT);

    root->add_style(&ui::style_container, LV_STATE_DEFAULT);
    root->set_style_bg_color(lv_color_make(150, 150, 150));
    root->set_style_pad_column(0);
    root->set_style_pad_row(0);

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);
    lv_obj_center(root);

    ProgramControlBuilder{this};
    ProgramListBuilder{this};

    lv_obj_set_grid_cell(programControl, LV_GRID_ALIGN_START , 0, 1, LV_GRID_ALIGN_START , 0, 1);
    lv_obj_set_grid_cell(programList,    LV_GRID_ALIGN_START , 0, 1, LV_GRID_ALIGN_START , 1, 1);
}

void MenuProgram::onEventDelete(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::onEventAppend);

    if(LV_EVENT_CLICKED == code)
    {
        obj->get_parent()->delete_sync();
    }
}

void MenuProgram::onEventAppend(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::onEventAppend);

    if(LV_EVENT_CLICKED == code)
    {
        ProgramListEntryBuilder{this_};
    }
}

void MenuProgram::onEventInsertUp(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::onEventInsertUp);
    auto event_list_entry = obj->get_parent();

    if(LV_EVENT_CLICKED == code)
    {
        auto i = event_list_entry->get_index();
        ProgramListEntryBuilder(this_).handle()->move_to_index(i);
    }
}

void MenuProgram::onEventInsertDown(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::onEventInsertDown);
    auto event_list_entry = obj->get_parent();

    if(LV_EVENT_CLICKED == code)
    {
        auto i = event_list_entry->get_index();
        ProgramListEntryBuilder(this_).handle()->move_to_index(i+1);
    }
}

void MenuProgram::onEventDebug(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto obj = (Object*) lv_event_get_target(e);
    auto this_ = (MenuProgram*) obj->get_event_user_data(&MenuProgram::onEventDebug);

    if(LV_EVENT_CLICKED == code)
    {
        auto list = this_->programList;
        auto N = list->get_child_cnt();
        for (uint32_t i=0; i<N; i++)
        {
            auto child = list->get_child(i);
            LV_LOG_USER("list[%u]: index=%u pointer=%p", i, child->get_index(), child);
        }
    }
}

void MenuProgram::onEventPlay(lv_event_t* e)
{}

void MenuProgram::onEventStop(lv_event_t* e)
{}

void MenuProgram::onEventLock(lv_event_t* e)
{}

} // namespace ui
