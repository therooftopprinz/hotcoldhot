#include "MenuProgram.hpp"

namespace ui
{

ProgramControlBuilder::ProgramControlBuilder(MenuProgram* program)
    : menu_program(program)
    , parent((Object*) program->get_tab())
{
    element = RectBuilder(parent).handle();
    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    element->set_style_pad_all(1, LV_STATE_DEFAULT);

    auto button_append = ButtonBuilder(element, on_event_append, this).handle();
    auto button_play = ButtonBuilder(element, on_event_play, this).handle();
    auto button_stop = ButtonBuilder(element, on_event_stop, this).handle();

    LabelBuilder(button_append, LV_SYMBOL_PLUS).handle();
    LabelBuilder(button_play, LV_SYMBOL_PLAY).handle();
    LabelBuilder(button_stop, LV_SYMBOL_STOP).handle();

    button_append->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
    button_play->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
    button_stop->set_height((CONFIG_SCREEN_LINE_HEIGHT*80)/100);
}

void ProgramControlBuilder::on_event_append(lv_event_t* e)
{
    auto this_ = (ProgramControlBuilder*) lv_event_get_user_data(e);
    this_->menu_program->on_event_append();
}

void ProgramControlBuilder::on_event_play(lv_event_t* e)
{
    auto this_ = (ProgramControlBuilder*) lv_event_get_user_data(e);
    this_->menu_program->on_event_play_pause();
}

void ProgramControlBuilder::on_event_stop(lv_event_t* e)
{
    auto this_ = (ProgramControlBuilder*) lv_event_get_user_data(e);
    this_->menu_program->on_event_stop();
}

ProgramEntryBuilder::ProgramEntryBuilder(MenuProgram* program)
    : menu_program(program)
    , parent((Object*) program->get_tab())
{
    element = RectBuilder(parent).handle();
    element->set_width(CONFIG_SCREEN_WIDTH);
    element->set_height(CONFIG_SCREEN_LINE_HEIGHT);
    element->add_style(&ui::style_container, LV_STATE_DEFAULT);
    element->set_style_pad_all(1, LV_STATE_DEFAULT);

    // auto row = lv_obj_create(list);
    // lv_obj_add_style(row, &style_border, 0);

    // lv_obj_set_layout(row, LV_LAYOUT_FLEX);
    // lv_obj_set_size(row, 480, LV_SIZE_CONTENT);
    // // lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    // // lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    // auto btn_insert    = lv_btn_create(row);
    // auto btn_delete    = lv_btn_create(row);
    // auto btn_move_up   = lv_btn_create(row);
    // auto btn_move_dn   = lv_btn_create(row);
    // TextArea* txa_duration = TextAreaBuilder(row, g_keypad).handle();
    // TextArea* txa_targetT  = TextAreaBuilder(row, g_keyboard).handle();
    // auto btn_run       = lv_btn_create(row);

    // lv_obj_set_size(txa_duration, 130, LV_SIZE_CONTENT);
    // lv_obj_set_size(txa_targetT,  130, LV_SIZE_CONTENT);

    // auto label_btn_insert = lv_label_create(btn_insert);
    // auto label_btn_delete = lv_label_create(btn_delete);
    // auto label_btn_move_up = lv_label_create(btn_move_up);
    // auto label_btn_move_dn = lv_label_create(btn_move_dn);
    // // auto label_txa_duration = lv_label_create(txa_duration);
    // // auto label_txa_targetT = lv_label_create(txa_targetT);
    // // auto label_btn_run = lv_label_create(btn_run);

    // // lv_textarea_set_one_line(txa_duration,true);
    // // lv_textarea_set_one_line(txa_targetT, true);

    // lv_label_set_text(label_btn_insert,    LV_SYMBOL_PLUS);
    // lv_label_set_text(label_btn_delete,    LV_SYMBOL_TRASH);
    // lv_label_set_text(label_btn_move_up,   LV_SYMBOL_UP);
    // lv_label_set_text(label_btn_move_dn,   LV_SYMBOL_DOWN);
    // // lv_label_set_text(btn_run,             LV_SYMBOL_RIGHT);
}

void ProgramEntryBuilder::on_event_sort(lv_event_t* e)
{}

void ProgramEntryBuilder::on_event_up(lv_event_t* e)
{}

void ProgramEntryBuilder::on_event_down(lv_event_t* e)
{}

void ProgramEntryBuilder::on_event_delete(lv_event_t* e)
{}

void ProgramEntryBuilder::on_event_play(lv_event_t* e)
{}

MenuProgram::MenuProgram(TabView* parent)
    : view(parent)
    , tab (parent->add_tab("Program"))
{
    tab->add_style(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    ProgramControlBuilder{this};
}

void MenuProgram::on_event_play_pause()
{}

void MenuProgram::on_event_lock_change(bool is_locked)
{}

void MenuProgram::on_event_stop()
{}

void MenuProgram::on_event_append()
{
}

Tab* MenuProgram::get_tab()
{
    return tab;
}

} // namespace ui
