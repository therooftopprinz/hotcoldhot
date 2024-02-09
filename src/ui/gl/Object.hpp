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

    void add_style(lv_style_t* style, lv_style_selector_t selector)
    {
        lv_obj_add_style(this, style, selector);
    }

    void remove_style(lv_style_t* style, lv_style_selector_t selector)
    {
        lv_obj_remove_style(this, style, selector);
    }

    void set_style_pad_all(lv_coord_t value, lv_style_selector_t selector)
    {
        lv_obj_set_style_pad_all(this, value, selector);
    }
};

} // namespace ui

#endif // __UI_OBJECT_HPP__