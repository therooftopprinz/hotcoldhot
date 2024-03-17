#ifndef __UI_OBJECT_HPP__
#define __UI_OBJECT_HPP__

#include "lvgl.h"

namespace ui
{

struct Object : lv_obj_t
{
    void show()
    {
        clearFlag(LV_OBJ_FLAG_HIDDEN);
    }

    void hide()
    {
        addFlag(LV_OBJ_FLAG_HIDDEN);
    }

    void clearFlag(lv_obj_flag_t flag)
    {
        lv_obj_clear_flag(this, flag);
    }

    void addFlag(lv_obj_flag_t flag)
    {
        lv_obj_add_flag(this, flag);
    }

    void setUserData(void* data)
    {
        lv_obj_set_user_data(this, data);
    }

    void* getUserData()
    {
        return lv_obj_get_user_data(this);
    }

    void addEventHandler(lv_event_cb_t cb, lv_event_code_t event_code, void* event_data = nullptr)
    {
        lv_obj_add_event_cb(this, cb, event_code, event_data);
    }

    void setWidth(lv_coord_t width)
    {
        lv_obj_set_width(this, width);
    }

    void setHeight(lv_coord_t height)
    {
        lv_obj_set_height(this, height);
    }

    void setContentWidth(lv_coord_t width)
    {
        lv_obj_set_content_width(this, width);
    }

    void setContentHeight(lv_coord_t height)
    {
        lv_obj_set_content_width(this, height);
    }

    void addStyle(lv_style_t* style, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_add_style(this, style, selector);
    }

    void removeStyle(lv_style_t* style, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_remove_style(this, style, selector);
    }

    void setStylePadAll(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_all(this, value, selector);
    }

    void setLayout(uint32_t layout)
    {
        lv_obj_set_layout(this, layout);
    }

    void setFlexFlow(lv_flex_flow_t flow)
    {
        lv_obj_set_flex_flow(this, flow);
    }

    void* getEventUserData(lv_event_cb_t cb)
    {
        return lv_obj_get_event_user_data(this, cb);
    }

    void setStyleBgColor(lv_color_t color, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_bg_color(this, color, selector);
    }

    void setStylePadColumn(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_column(this, value, LV_STATE_DEFAULT);
    }

    void setStylePadRow(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_row(this, value, LV_STATE_DEFAULT);
    }

    void setFlexAlign(lv_flex_align_t main_place, lv_flex_align_t cross_place,
                           lv_flex_align_t track_place)
    {
        lv_obj_set_flex_align(this, main_place, cross_place, track_place);
    }

    Object* getParent()
    {
        return (Object*) lv_obj_get_parent(this);
    }

    void deleteSync()
    {
        return lv_obj_del(this);
    }

    void deleteAsync()
    {
        return lv_obj_del_async(this);
    }

    uint32_t getChildCnt()
    {
        return lv_obj_get_child_cnt(this);
    }

    void moveToIndex(int32_t n)
    {
        lv_obj_move_to_index(this, n);
    }

    int32_t getIndex()
    {
        return lv_obj_get_index(this);
    }

    Object* getChild(int32_t i)
    {
        return (Object*) lv_obj_get_child(this, i);
    }
};

} // namespace ui

#endif // __UI_OBJECT_HPP__