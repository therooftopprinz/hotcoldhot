#include "MenuRun.hpp"
#include "string_utils.hpp"

namespace ui
{
MenuRun::MenuRun(UI& ui, TabView *parent)
    : ui(ui)
    , view(parent)
{
    auto tab = parent->addTab("Run");
    tab->addStyle(&ui::style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    tab->setStylePadColumn(CPAD);
    tab->setStylePadRow(RPAD);

    root = tab;

    constexpr int chart_width = ((CONFIG_SCREEN_WIDTH * 6.5) / 10 - CPAD);
    /*Create a container with grid*/
    static lv_coord_t col_dsc[] = {
        chart_width,                                   // Chart
        (int(CONFIG_SCREEN_WIDTH * 1.0) / 10 - CPAD), // Text1
        (int(CONFIG_SCREEN_WIDTH * 0.79) / 10 - CPAD), // Text2
        (int(CONFIG_SCREEN_WIDTH * 0.79) / 10 - CPAD), // Text3
        (int(CONFIG_SCREEN_WIDTH * 0.79) / 10 - CPAD), // Text4
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

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);

    lv_obj_set_grid_cell(LabelBuilder(root, "State").handle(),                   LV_GRID_ALIGN_START,  1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Target").handle(),                  LV_GRID_ALIGN_START,  1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Left").handle(),                    LV_GRID_ALIGN_START,  1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Mode").handle(),                    LV_GRID_ALIGN_START,  1, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "PWM").handle(),                     LV_GRID_ALIGN_START,  3, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "TLeft").handle(),                   LV_GRID_ALIGN_START,  1, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_set_grid_cell(state   = LabelBuilder(root, "STATE").handle(),         LV_GRID_ALIGN_END,    2, 3, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(targetN = LabelBuilder(root, "N-R").handle(),             LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(leftH   = LabelBuilder(root, "HH:MM:SS").handle(),      LV_GRID_ALIGN_CENTER, 2, 2, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(mode    = LabelBuilder(root, "M").handle(),             LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(tleftH  = LabelBuilder(root, "HH:MM:SS").handle(),      LV_GRID_ALIGN_CENTER, 2, 2, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_set_grid_cell(targetT = LabelBuilder(root, "TTT.T/TTT.T").handle(),   LV_GRID_ALIGN_END,    4, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(leftS   = LabelBuilder(root, "SSSS").handle(),          LV_GRID_ALIGN_END,    4, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(pwm     = LabelBuilder(root, "XXX").handle(),           LV_GRID_ALIGN_END,    4, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(tleftS  = LabelBuilder(root, "SSSS").handle(),          LV_GRID_ALIGN_END,    4, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_set_grid_cell(ButtonBuilder(root, "Start", &onClickStart, this).handle(), LV_GRID_ALIGN_CENTER, 3, 2, LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, "Stop",  &onClickStop,  this).handle(), LV_GRID_ALIGN_CENTER, 3, 2, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_PLUS,  nullptr).handle(),     LV_GRID_ALIGN_CENTER, 1, 2, LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_MINUS, nullptr).handle(),     LV_GRID_ALIGN_CENTER, 1, 2, LV_GRID_ALIGN_CENTER, 6, 1);

    // create chart
    chart = (Object*) lv_chart_create(root);
    lv_obj_set_size(chart, chart_width, CONFIG_SCREEN_LINE_HEIGHT*7-2*RPAD);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -2000, 12000);
    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 0);

    serTarget = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    serCurrent = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_ext_y_array(chart, serTarget, nullptr);
    lv_chart_set_ext_y_array(chart, serCurrent, nullptr);

    lv_obj_set_grid_cell(chart, LV_GRID_ALIGN_END,    0, 1, LV_GRID_ALIGN_START, 0, 7);
}

void MenuRun::updateChart(const std::tuple<short*,size_t,size_t>& target, const std::tuple<short*,size_t,size_t>& current)
{
    auto serTargetStart  = lv_chart_get_x_start_point(chart, serTarget);
    auto serCurrentStart = lv_chart_get_x_start_point(chart, serCurrent);
    auto chartCount = lv_chart_get_point_count(chart);
    auto serTargetData  = lv_chart_get_y_array(chart, serTarget);
    auto serCurrentData = lv_chart_get_y_array(chart, serCurrent);

    auto bufTargetStart = std::get<2>(target);
    auto bufCurrentStart = std::get<2>(current);
    auto bufCount = std::min(std::get<1>(target), std::get<1>(current));
    auto bufTargetData = std::get<0>(target);
    auto bufCurrentData = std::get<0>(current);

    bool doRefresh = false;

    if (serTargetData != bufTargetData)
    {
        lv_chart_set_ext_y_array(chart, serTarget, bufTargetData);
        doRefresh = true;
    }

    if (serCurrentData != bufCurrentData)
    {
        lv_chart_set_ext_y_array(chart, serCurrent, bufCurrentData);
        doRefresh = true;
    }

    if (chartCount != bufCount)
    {
        lv_chart_set_point_count(chart, bufCount);
        doRefresh = true;
    }

    if (serTargetStart != bufTargetStart)
    {
        lv_chart_set_x_start_point(chart, serTarget, bufTargetStart);
        doRefresh = true;
    }

    if (serCurrentStart != bufCurrentStart)
    {
        lv_chart_set_x_start_point(chart, serCurrent, bufCurrentStart);
        doRefresh = true;
    }

    if (doRefresh)
    {
        lv_chart_refresh(chart);
    }
}

void MenuRun::onStatus(const IApp::status_t& status)
{
    state->setText("%s", status.state);
    targetT->setText("%.1f/%.1f",
        status.currentActualT,
        status.currentTargetT.value_or(status.currentActualT));
    if ('I' != status.state[0])
    {
        if (status.repn)
        {
            targetN->setText("T%d-R%d",
                status.currentTargetN.value_or(-1)+1,
                status.repn.value_or(0));
        }
        else
        {
            targetN->setText("T%d",
                status.currentTargetN.value_or(-1)+1);
        }
    }
    else
    {
        targetN->setText("");
    }

    if (status.totalRemaining)
    {
        auto rem = *status.totalRemaining;
        tleftS->setText("%d", rem);
        char hhmmss[9];
        to_s_str_hhmmss(hhmmss, sizeof(hhmmss), rem);
        tleftS->setText(hhmmss);
    }
    else
    {
        tleftS->setText("");
        tleftH->setText("");
    }


    if (status.currentRemaining)
    {
        auto rem = *status.currentRemaining;
        leftS->setText("%d", rem);
        char hhmmss[9];
        to_s_str_hhmmss(hhmmss, sizeof(hhmmss), rem);
        leftS->setText(hhmmss);
    }
    else
    {
        leftS->setText("");
        leftH->setText("");
    }

    pwm->setText("%d", status.pwm);
}

void MenuRun::onClickStart(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto res = this_->ui.validate();
        if (!res.first)
        {
            MessageBoxBuilder(this_->ui.getGroup())
                .title("Validate Failed")
                .text("Please check program!")
                .buttons(MessageBoxBuilder::BUTTONS_OK)
                .show();
        }
        else
        {
            this_->ui.start(res.second);
            MessageBoxBuilder(this_->ui.getGroup())
                .title("Notification")
                .text("Started!")
                .buttons(MessageBoxBuilder::BUTTONS_OK)
                .show();
        }
    }
}

void MenuRun::onClickStop(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        this_->ui.stop();
    }
}

} // namespace ui
