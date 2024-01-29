#include "TabView.hpp"

namespace ui
{

TabViewBuilder::TabViewBuilder(lv_obj_t* parent, lv_dir_t pos, lv_coord_t size)
    : element((TabView*)lv_tabview_create(parent, pos, size))
{}

TabView* TabViewBuilder::handle()
{
    return element;
}

} // namespace ui