#ifndef __UI_MENUPROGRAM_HPP__
#define __UI_MENUPROGRAM_HPP__

#include <cstdio>

#include "lvgl.h"

#include "config.h"

#include <ui/UI.hpp>

#include <ui/gl/TextArea.hpp>
#include <ui/gl/Keyboard.hpp>
#include <ui/gl/TabView.hpp>
#include <ui/gl/Rect.hpp>

#include <ui/GlobalStyles.hpp>

namespace ui
{

class ProgramControlBuilder
{
public:
    ProgramControlBuilder() = delete;
    ProgramControlBuilder(lv_obj_t* parent)
        : parent(parent)
    {
        auto element = RectBuilder(parent).handle();
        element->set_content_width(CONFIG_SCREEN_WIDTH);
        element->set_content_height(CONFIG_SCREEN_LINE_HEIGHT);
    }
private:
    lv_obj_t* parent = nullptr; 
    lv_obj_t* element = nullptr;
};

class MenuProgram
{
public:
    void new_row()
    {
        // auto row = lv_obj_create(list);
        // // lv_obj_add_style(row, &style_border, 0);

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

    MenuProgram(TabView* parent)
        : view(parent)
        , tab (parent->add_tab("Program"))
    {
        ProgramControlBuilder{tab};
    }

private:
    TabView* view = nullptr;
    Tab* tab = nullptr;
};

} // namespace ui

#endif // __UI_MENUPROGRAM_HPP__