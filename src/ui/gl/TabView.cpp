#include "TabView.hpp"

namespace ui
{

Tab* TabView::addTab( const char* name)
{
    return (Tab*) lv_tabview_add_tab(this, name);
}

TabViewBuilder::TabViewBuilder(lv_obj_t* parent, lv_dir_t pos, lv_coord_t size)
    : element((TabView*)lv_tabview_create(parent, pos, size))
{}

TabView* TabViewBuilder::handle()
{
    return element;
}

} // namespace ui