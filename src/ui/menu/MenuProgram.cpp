#include "MenuProgram.hpp"

#include <sstream>
#include <cmath>

namespace ui
{

MenuProgram::MenuProgram(UI& ui, TabView* parent)
    : ui(ui)
    , view(parent)
{
    auto tab = parent->addTab("Program");
    tab->addStyle(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    tab->setStylePadColumn(CPAD);
    tab->setStylePadRow(RPAD);

    /*Create a container with grid*/
    static lv_coord_t col_dsc[] = {
                                    ((CONFIG_SCREEN_WIDTH*1)/10-CPAD), // T text
                                    ((CONFIG_SCREEN_WIDTH*2)/10-CPAD), // T input
                                    ((CONFIG_SCREEN_WIDTH*1)/10-CPAD), // t text
                                    ((CONFIG_SCREEN_WIDTH*2)/10-CPAD), // t input
                                    ((CONFIG_SCREEN_WIDTH*4)/10-CPAD), // keypad
                                    LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    CONFIG_SCREEN_LINE_HEIGHT - RPAD,
                                    LV_GRID_TEMPLATE_LAST};

    root = tab;
    root->setWidth(CONFIG_SCREEN_WIDTH);
    root->setHeight(CONFIG_SCREEN_HEIGHT - CONFIG_SCREEN_LINE_HEIGHT);

    root->addStyle(&ui::style_container, LV_STATE_DEFAULT);
    root->setStylePadColumn(5);
    root->setStylePadRow(5);

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);
    lv_obj_center(root);
    
    // keypad
    auto keypad = KeyboardBuilder(root).handle();
    keypad->setMode(LV_KEYBOARD_MODE_NUMBER);
    keypad->setWidth((CONFIG_SCREEN_WIDTH*4)/10-CPAD*2);
    keypad->setHeight(CONFIG_SCREEN_LINE_HEIGHT*7-CPAD*2);

    lv_obj_set_grid_cell(keypad, LV_GRID_ALIGN_START , 4, 1, LV_GRID_ALIGN_START , 0, 7);

    lv_obj_set_grid_cell(LabelBuilder(root, "Temperature").handle(), LV_GRID_ALIGN_CENTER , 0, 2, LV_GRID_ALIGN_CENTER , 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T1").handle(),          LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T5").handle(),          LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T2").handle(),          LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T3").handle(),          LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 5, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "T4").handle(),          LV_GRID_ALIGN_CENTER , 0, 1, LV_GRID_ALIGN_CENTER , 6, 1);

    lv_obj_set_grid_cell(inTemp[0] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(inTemp[4] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(inTemp[1] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(inTemp[2] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 5, 1);
    lv_obj_set_grid_cell(inTemp[3] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 1, 1, LV_GRID_ALIGN_CENTER , 6, 1);

    lv_obj_set_grid_cell(LabelBuilder(root, "Time").handle(),  LV_GRID_ALIGN_CENTER , 2, 2, LV_GRID_ALIGN_CENTER , 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t1").handle(),    LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t5").handle(),    LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Count").handle(), LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t2").handle(),    LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t3").handle(),    LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 5, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "t4").handle(),    LV_GRID_ALIGN_CENTER , 2, 1, LV_GRID_ALIGN_CENTER , 6, 1);
   
    lv_obj_set_grid_cell(inTime[0] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 1, 1);
    lv_obj_set_grid_cell(inTime[4] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 2, 1);
    lv_obj_set_grid_cell(inCount   = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 3, 1);
    lv_obj_set_grid_cell(inTime[1] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 4, 1);
    lv_obj_set_grid_cell(inTime[2] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 5, 1);
    lv_obj_set_grid_cell(inTime[3] = TextAreaBuilder(root, 75, 0, keypad, false).handle(), LV_GRID_ALIGN_CENTER , 3, 1, LV_GRID_ALIGN_CENTER , 6, 1);
}

static float getFloat(const char* in)
{
    float rv;
    std::stringstream ss(in);
    ss >> rv;
    if (ss.fail())
    {
        return nanf(in);
    }
    return rv;
}

std::pair<bool,int> getInt(const char* in)
{
    int rv;
    std::stringstream ss(in);
    ss >> rv;
    if (ss.fail())
    {
        return {false, -1};
    }
    return {true, rv};
}

std::pair<bool, IApp::program_t> MenuProgram::validate()
{
    std::pair<bool, IApp::program_t> rv;
    unsigned errors = 0;
    for (unsigned i=0; i<5; i++)
    {
        auto temp = getFloat(inTemp[i]->getText());
        auto time = getInt(inTime[i]->getText());

        if (isnanf(temp))
        {
            errors++;
            inTemp[i]->setStyleBgColor(lv_color_hex(0xFF7C7C));
        }
        else
        {
            inTemp[i]->setStyleBgColor(lv_color_hex(0xFFFFFF));
        }

        if (!time.first)
        {
            errors++;
            inTime[i]->setStyleBgColor(lv_color_hex(0xFF7C7C));
        }
        else
        {
            inTime[i]->setStyleBgColor(lv_color_hex(0xFFFFFF));
        }

        rv.second.ttConfig[i].first = temp; 
        rv.second.ttConfig[i].second = time.second;
    }

    auto count = getInt(inCount->getText());
    if (!count.first)
    {
        errors++;
        inCount->setStyleBgColor(lv_color_hex(0xFF7C7C));
    }
    else
    {
        inCount->setStyleBgColor(lv_color_hex(0xFFFFFF));
    }

    rv.second.rep_cnt = count.second;
    rv.first = !errors;

    return rv;
}

} // namespace ui
