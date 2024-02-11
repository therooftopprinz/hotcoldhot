#ifndef __UI_OBJECT_HPP__
#define __UI_OBJECT_HPP__

#include "lvgl.h"

namespace ui
{

struct Object : lv_obj_t
{
    void show()
    {
        lv_obj_clear_flag(this, LV_OBJ_FLAG_HIDDEN);
    }

    void hide()
    {
        lv_obj_add_flag(this, LV_OBJ_FLAG_HIDDEN);
    }

    void set_user_data(void* data)
    {
        lv_obj_set_user_data(this, data);
    }

    void* get_user_data()
    {
        return lv_obj_get_user_data(this);
    }

    void add_event_handler(lv_event_cb_t cb, lv_event_code_t event_code, void* event_data = nullptr)
    {
        lv_obj_add_event_cb(this, cb, event_code, event_data);
    }

    void set_width(lv_coord_t width)
    {
        lv_obj_set_width(this, width);
    }

    void set_height(lv_coord_t height)
    {
        lv_obj_set_height(this, height);
    }

    void set_content_width(lv_coord_t width)
    {
        lv_obj_set_content_width(this, width);
    }

    void set_content_height(lv_coord_t height)
    {
        lv_obj_set_content_width(this, height);
    }

    void add_style(lv_style_t* style, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_add_style(this, style, selector);
    }

    void remove_style(lv_style_t* style, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_remove_style(this, style, selector);
    }

    void set_style_pad_all(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_all(this, value, selector);
    }

    void set_layout(uint32_t layout)
    {
        lv_obj_set_layout(this, layout);
    }

    void set_flex_flow(lv_flex_flow_t flow)
    {
        lv_obj_set_flex_flow(this, flow);
    }

    void* get_event_user_data(lv_event_cb_t cb)
    {
        return lv_obj_get_event_user_data(this, cb);
    }

    void set_style_bg_color(lv_color_t color, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_bg_color(this, color, selector);
    }

    void set_style_pad_column(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_column(this, value, LV_STATE_DEFAULT);
    }

    void set_style_pad_row(lv_coord_t value, lv_style_selector_t selector=LV_STATE_DEFAULT)
    {
        lv_obj_set_style_pad_row(this, value, LV_STATE_DEFAULT);
    }

    void set_flex_align(lv_flex_align_t main_place, lv_flex_align_t cross_place,
                           lv_flex_align_t track_place)
    {
        lv_obj_set_flex_align(this, main_place, cross_place, track_place);
    }

    Object* get_parent()
    {
        return (Object*) lv_obj_get_parent(this);
    }

    void delete_sync()
    {
        return lv_obj_del(this);
    }

    void delete_async()
    {
        return lv_obj_del_async(this);
    }

    uint32_t get_child_cnt()
    {
        return lv_obj_get_child_cnt(this);
    }

    void move_to_index(int32_t n)
    {
        lv_obj_move_to_index(this, n);
    }

    int32_t get_index()
    {
        return lv_obj_get_index(this);
    }

    Object* get_child(int32_t i)
    {
        return (Object*) lv_obj_get_child(this, i);
    }
};

} // namespace ui

#endif // __UI_OBJECT_HPP__