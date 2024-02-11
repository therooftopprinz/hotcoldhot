#ifndef __UI_TABVIEW_HPP__
#define __UI_TABVIEW_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct Tab : Object{};
struct TabView : Object
{
    Tab* add_tab( const char* name)
    {
        return (Tab*) lv_tabview_add_tab(this, name);
    }
};

class TabViewBuilder
{
public:
    TabViewBuilder() = delete;
    TabViewBuilder(lv_obj_t* parent, lv_dir_t pos, lv_coord_t size);
    TabView* handle();

private:
    static void on_event(lv_event_t* e);
    TabView* element = nullptr;
};


} // namespace ui

#endif // __UI_TABVIEW_HPP__