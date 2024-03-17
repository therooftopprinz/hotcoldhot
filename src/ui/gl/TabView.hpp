#ifndef __UI_TABVIEW_HPP__
#define __UI_TABVIEW_HPP__

#include "lvgl.h"

#include "Object.hpp"

namespace ui
{

struct Tab : Object{};
struct TabView : Object
{
    Tab* addTab( const char* name);

    template<typename T>
    T* getContent()
    {
        return (T*) lv_tabview_get_content(this);
    }
};

class TabViewBuilder
{
public:
    TabViewBuilder() = delete;
    TabViewBuilder(lv_obj_t* parent, lv_dir_t pos, lv_coord_t size);
    TabView* handle();

private:
    TabView* element = nullptr;
};


} // namespace ui

#endif // __UI_TABVIEW_HPP__