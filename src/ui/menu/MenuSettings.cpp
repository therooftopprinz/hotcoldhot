
#include "MenuSettings.hpp"
#include <ui/gl/Rect.hpp>
#include <ui/gl/Button.hpp>
#include <configmap.hpp>

namespace ui
{


MenuSettings::MenuSettings(UI&,TabView* parent)
    : ui(ui)
    , view(parent)
{
    auto tab = parent->addTab("Settings");
    tab->addStyle(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    tab->setStylePadColumn(CPAD);
    tab->setStylePadRow(RPAD);

    /*Create a container with grid*/
    static lv_coord_t col_dsc[] = {
                                    CONFIG_SCREEN_WIDTH/2,
                                    CONFIG_SCREEN_WIDTH/2,
                                    LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {
                                    CONFIG_SCREEN_LINE_HEIGHT,
                                    CONFIG_SCREEN_LINE_HEIGHT*6,
                                    LV_GRID_TEMPLATE_LAST};

    root = tab;
    root->setWidth(CONFIG_SCREEN_WIDTH);
    root->setHeight(CONFIG_SCREEN_HEIGHT - CONFIG_SCREEN_LINE_HEIGHT);

    root->addStyle(&ui::style_container, LV_STATE_DEFAULT);
    root->setStylePadAll(0);
    root->setStylePadColumn(0);
    root->setStylePadRow(0);

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);
    lv_obj_center(root);

    // keypad
    kp = KeyboardBuilder(root).handle();
    kp->setMode(LV_KEYBOARD_MODE_NUMBER);
    kp->setWidth(CONFIG_SCREEN_WIDTH/2);
    kp->setHeight(CONFIG_SCREEN_LINE_HEIGHT*6);

    lv_obj_set_grid_cell(kp                                 , LV_GRID_ALIGN_START , 1, 1, LV_GRID_ALIGN_START , 0, 6);
    lv_obj_set_grid_cell(ctrls  = RectBuilder(root).handle(), LV_GRID_ALIGN_START , 0, 1, LV_GRID_ALIGN_START , 0, 1);
    lv_obj_set_grid_cell(fields = RectBuilder(root).handle(), LV_GRID_ALIGN_START , 0, 1, LV_GRID_ALIGN_START , 1, 6);

    ctrls->setStylePadAll(0);
    fields->setStylePadAll(0);

    ctrls->setWidth(CONFIG_SCREEN_WIDTH/2);
    ctrls->setHeight(CONFIG_SCREEN_LINE_HEIGHT);
    fields->setWidth(CONFIG_SCREEN_WIDTH/2);
    fields->setHeight(CONFIG_SCREEN_LINE_HEIGHT*6);

    ctrls->setFlexFlow(LV_FLEX_FLOW_ROW);
    fields->setFlexFlow(LV_FLEX_FLOW_COLUMN);

    //     ButtonBuilder(lv_obj_t* parent, const char* label, lv_event_cb_t cb,  void* user_data = nullptr, lv_event_code_t = LV_EVENT_CLICKED);
    auto save = ButtonBuilder(ctrls, LV_SYMBOL_SAVE, onClickSave, this).handle();
    auto load = ButtonBuilder(ctrls, LV_SYMBOL_DIRECTORY, onClickLoad, this).handle();
}

void MenuSettings::onClickSave(lv_event_t* e)
{
    auto this_   = (MenuSettings*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        std::fstream fs(std::string(FS_PREFIX) + "/config.cfg", fs.trunc | fs.out);
        for(unsigned i=0 ; i<this_->fields->getChildCnt(); i++)
        {
            auto rect = (Rect*) this_->fields->getChild(i);
            auto label = (Label*) rect->getChild(0);
            auto text = (TextArea*) rect->getChild(1);

            fs << label->getText() << " = ";
            fs << text->getText() << "\n";
        }
    }
}

void MenuSettings::onClickLoad(lv_event_t* e)
{
    auto this_   = (MenuSettings*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        while(this_->fields->getChildCnt())
        {
            this_->fields->getChild(0)->deleteSync();
        }
        configmap cm;
        cm.load(FS_PREFIX "/config.cfg");

        auto& cfg = cm.get();

        for (auto& e : cfg)
        {
            LV_LOG_USER("config: %s = %s", e.first.c_str(), e.second.raw().c_str());
            auto rect = RectBuilder(this_->fields).handle();
            rect->setStylePadAll(0);
            rect->setStylePadColumn(0);
            rect->setStylePadRow(0);
            rect->setWidth(CONFIG_SCREEN_WIDTH/2 - CPAD);
            rect->setHeight(CONFIG_SCREEN_LINE_HEIGHT);
            rect->setFlexFlow(LV_FLEX_FLOW_ROW);
            rect->setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            rect->clearFlag(LV_OBJ_FLAG_SCROLLABLE);
            auto label = LabelBuilder(rect, e.first.c_str()).handle();
            auto text = TextAreaBuilder(rect, 75, 0, this_->kp, false).handle();
            text->setText(e.second.raw().c_str());
            label->setWidth(CONFIG_SCREEN_WIDTH/4-CPAD);
            text->setWidth(CONFIG_SCREEN_WIDTH/4-CPAD);
        }
    }
}

} // namespace ui
