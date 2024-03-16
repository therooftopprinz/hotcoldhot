#include "MenuProgram.hpp"

namespace ui
{

MenuProgram::MenuProgram(TabView* parent)
    : view(parent)
{
    constexpr auto CPAD = 5;
    constexpr auto RPAD = 5;

    auto tab = parent->add_tab("Program");
    tab->add_style(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    tab->set_style_pad_column(CPAD);
    tab->set_style_pad_row(RPAD);

    /*Create a container with grid*/
    static lv_coord_t col_dsc[] = {
                                    (CONFIG_SCREEN_WIDTH*1)/10-CPAD, // T text
                                    (CONFIG_SCREEN_WIDTH*2)/10-CPAD, // T input
                                    (CONFIG_SCREEN_WIDTH*1)/10-CPAD, // t text
                                    (CONFIG_SCREEN_WIDTH*2)/10-CPAD, // t input
                                    (CONFIG_SCREEN_WIDTH*4)/10-CPAD, // keypad
                                    LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    LV_GRID_TEMPLATE_LAST};

    root = (Object*) lv_obj_create(tab);
    root->set_width(CONFIG_SCREEN_WIDTH);
    root->set_height(CONFIG_SCREEN_HEIGHT - CONFIG_SCREEN_LINE_HEIGHT);

    root->add_style(&ui::style_container, LV_STATE_DEFAULT);
    root->set_style_bg_color(lv_color_make(150, 150, 150));
    root->set_style_pad_column(5);
    root->set_style_pad_row(5);

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);
    lv_obj_center(root);
    
    // keypad
    auto keypad = KeyboardBuilder(root).handle();
    keypad->set_mode(LV_KEYBOARD_MODE_NUMBER);
    keypad->set_width((CONFIG_SCREEN_WIDTH*4)/10);
    keypad->set_height(CONFIG_SCREEN_LINE_HEIGHT*7);

    lv_obj_set_grid_cell(keypad, LV_GRID_ALIGN_START , 4, 1, LV_GRID_ALIGN_START , 0, 7);

    lv_obj_set_grid_cell(LabelBuilder(root, "Temperature").handle(), LV_GRID_ALIGN_CENTER , 0, 2, LV_GRID_ALIGN_CENTER , 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T1").handle(), LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T2").handle(), LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T3").handle(), LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T4").handle(), LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T5").handle(), LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 5, 1);

    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 5, 1);

    lv_obj_set_grid_cell(LabelBuilder(root, "Time").handle(), LV_GRID_ALIGN_CENTER , 2, 2, LV_GRID_ALIGN_CENTER , 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t1").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t2").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t3").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t4").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t5").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 5, 1);

    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 5, 1);
}

} // namespace ui
