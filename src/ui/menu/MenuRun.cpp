#include "MenuRun.hpp"
#include "string_utils.hpp"
#include <cmath>

namespace ui
{
MenuRun::MenuRun(UI& ui, IApp& app, MenuProgram& program, TabView* parent)
    : ui(ui)
    , app(app)
    , program(program)
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
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 0 chart1
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 1 chart2
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 2 chart3
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 3 chart4
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 4 chart5
        (int(CONFIG_SCREEN_WIDTH * 1.095) / 10 - CPAD), // 5 chart6
        (int(CONFIG_SCREEN_WIDTH * 0.980) / 10 - CPAD), // 6 Text1
        (int(CONFIG_SCREEN_WIDTH * 0.750) / 10 - CPAD), // 7 Text2
        (int(CONFIG_SCREEN_WIDTH * 0.750) / 10 - CPAD), // 8 Text3
        (int(CONFIG_SCREEN_WIDTH * 0.750) / 10 - CPAD), // 9 Text4
        LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 0
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 1
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 2
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 3
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 4
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 5
        CONFIG_SCREEN_LINE_HEIGHT - RPAD, // 6
        LV_GRID_TEMPLATE_LAST};

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);

    constexpr auto TXT_COL = 5;

    lv_obj_set_grid_cell(LabelBuilder(root, "State").handle(),                   LV_GRID_ALIGN_START,  TXT_COL + 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Target").handle(),                  LV_GRID_ALIGN_START,  TXT_COL + 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Current").handle(),                 LV_GRID_ALIGN_START,  TXT_COL + 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Mode").handle(),                    LV_GRID_ALIGN_START,  TXT_COL + 1, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Power").handle(),                   LV_GRID_ALIGN_START,  TXT_COL + 3, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(LabelBuilder(root, "Total").handle(),                   LV_GRID_ALIGN_START,  TXT_COL + 1, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_set_grid_cell(state   = LabelBuilder(root, "STATE").handle(),         LV_GRID_ALIGN_END,    TXT_COL + 2, 3, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(targetN = LabelBuilder(root, "N-R").handle(),           LV_GRID_ALIGN_CENTER, TXT_COL + 2, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(leftH   = LabelBuilder(root, "HH:MM:SS").handle(),      LV_GRID_ALIGN_CENTER, TXT_COL + 2, 2, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(mode    = (Object*) lv_led_create(root),                LV_GRID_ALIGN_CENTER, TXT_COL + 2, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(tleftH  = LabelBuilder(root, "HH:MM:SS").handle(),      LV_GRID_ALIGN_CENTER, TXT_COL + 2, 2, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_set_grid_cell(targetT = LabelBuilder(root, "TTT.T/TTT.T").handle(),   LV_GRID_ALIGN_END,    TXT_COL + 4, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(leftS   = LabelBuilder(root, "SSSS").handle(),          LV_GRID_ALIGN_END,    TXT_COL + 4, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(pwm     = LabelBuilder(root, "XXX").handle(),           LV_GRID_ALIGN_END,    TXT_COL + 4, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_set_grid_cell(tleftS  = LabelBuilder(root, "SSSS").handle(),          LV_GRID_ALIGN_END,    TXT_COL + 4, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    // Play Stop
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_PLAY, &onClickStart, this).handle(),  LV_GRID_ALIGN_CENTER, TXT_COL + 3, 2, LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_STOP,  &onClickStop,  this).handle(), LV_GRID_ALIGN_CENTER, TXT_COL + 3, 2, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, "Load",  nullptr).handle(),                     LV_GRID_ALIGN_CENTER, TXT_COL + 1, 2, LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, "Save", nullptr).handle(),                      LV_GRID_ALIGN_CENTER, TXT_COL + 1, 2, LV_GRID_ALIGN_CENTER, 6, 1);

    // Grap Control
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_LEFT,  &onClickScrollLeft, this).handle(),    LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_RIGHT, &onClickScrollRight, this).handle(),   LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_PLUS,  &onClickScrollPlus, this).handle(),    LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_MINUS, &onClickScrollMinus, this).handle(),   LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_PREV,  &onClickScrollFirst, this).handle(),   LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(ButtonBuilder(root, LV_SYMBOL_NEXT,  &onClickScrollLast, this).handle(),    LV_GRID_ALIGN_CENTER, 5, 1, LV_GRID_ALIGN_CENTER, 6, 1);

    // set led
    lv_led_set_brightness(mode, 0);
    lv_led_set_color(mode, lv_palette_main(LV_PALETTE_RED));

    // create chart
    chart = (Object*) lv_chart_create(root);
    lv_obj_set_size(chart, chart_width, CONFIG_SCREEN_LINE_HEIGHT*6-2*RPAD);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -2000, 12000);
    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 0);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

    serTarget = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    serCurrent = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_ext_y_array(chart, serTarget, nullptr);
    lv_chart_set_ext_y_array(chart, serCurrent, nullptr);

    lv_obj_set_grid_cell(chart, LV_GRID_ALIGN_END,    0, 6, LV_GRID_ALIGN_START, 0, 6);

    updateZoom();
}

void MenuRun::updateZoom()
{
    auto& bufCurrent = app.getChartActual();
    auto& bufTarget = app.getChartTarget();
    bufCurrent.setWindowSize(scrollConst*pow(2, scrollZoomLevel));
    bufTarget.setWindowSize (scrollConst*pow(2, scrollZoomLevel));
    if (!updateChart())
    {
        lv_chart_refresh(chart);
    }
}

void MenuRun::loop()
{
    updateStatus();
    updateChart();
}

bool MenuRun::updateChart()
{
    auto& bufTarget = app.getChartTarget();
    auto& bufCurrent = app.getChartActual();

    auto serTargetStart  = lv_chart_get_x_start_point(chart, serTarget);
    auto serCurrentStart = lv_chart_get_x_start_point(chart, serCurrent);
    auto chartCount = lv_chart_get_point_count(chart);
    auto serTargetData  = lv_chart_get_y_array(chart, serTarget);
    auto serCurrentData = lv_chart_get_y_array(chart, serCurrent);

    auto bufTargetStart = bufTarget.getOffset();
    auto bufTargetData = bufTarget.getBuffer();
    auto bufCurrentStart = bufCurrent.getOffset();
    auto bufCurrentData = bufCurrent.getBuffer();
    auto bufCount = std::min(bufTarget.getWindowSize(), bufCurrent.getWindowSize());

    bool doRefresh = false;

    if (serTargetData != bufTargetData)
    {
        LV_LOG_USER("MenuRun | new target data %p to %p", serTargetData, bufTargetData);
        lv_chart_set_ext_y_array(chart, serTarget, bufTargetData);
        doRefresh = true;
    }

    if (serCurrentData != bufCurrentData)
    {
        LV_LOG_USER("MenuRun | new actual data %p to %p", serCurrentData, bufCurrentData);
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
        LV_LOG_USER("MenuRun | new target start %hu to %lu", serTargetStart, bufTargetStart);
        lv_chart_set_x_start_point(chart, serTarget, bufTargetStart);
        doRefresh = true;
    }

    if (serCurrentStart != bufCurrentStart)
    {
        LV_LOG_USER("MenuRun | new actual start %hu to %lu", serCurrentStart, bufCurrentStart);
        lv_chart_set_x_start_point(chart, serCurrent, bufCurrentStart);
        doRefresh = true;
    }

    if (doRefresh)
    {
        lv_chart_refresh(chart);
    }

    return doRefresh;
}

void MenuRun::updateStatus()
{
    const IApp::status_t& status = app.status();

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

        lv_led_set_brightness(mode, 128*fabs(status.pwm)/100+127);
        auto color = status.pwm > 0 ? LV_PALETTE_BLUE : LV_PALETTE_RED;
        lv_led_set_color(mode, lv_palette_main(color));
    }
    else
    {
        lv_led_set_brightness(mode, 0);
        lv_led_set_color(mode, lv_palette_main(LV_PALETTE_GREY));
        targetN->setText("");
    }

    if (status.totalRemaining)
    {
        auto rem = *status.totalRemaining;
        tleftS->setText("%d", rem);
        char hhmmss[9];
        to_s_str_hhmmss(hhmmss, sizeof(hhmmss), rem);
        tleftH->setText(hhmmss);
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
        leftH->setText(hhmmss);
    }
    else
    {
        leftS->setText("");
        leftH->setText("");
    }

    pwm->setText("%d", abs(status.pwm));
}

void MenuRun::onClickStart(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto res = this_->program.validate();
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
            this_->app.start(res.second);
        }
    }
}

void MenuRun::onClickStop(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        this_->app.stop();
    }
}

void MenuRun::onClickScrollLeft(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto& bufCurrent = this_->app.getChartActual();
        auto& bufTarget = this_->app.getChartTarget();
        int64_t current = scrollConst*pow(2, this_->scrollZoomLevel)/10;
        LV_LOG_USER("MenuRun | scroll: %ld (off=%ld,%ld)",
            -current, bufCurrent.getFileOffset(), bufTarget.getFileOffset());
        bufCurrent.scrollBy(-current);
        bufTarget.scrollBy(-current);
        if (!this_->updateChart())
        {
            lv_chart_refresh(this_->chart);
        }
    }
}

void MenuRun::onClickScrollRight(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto& bufCurrent = this_->app.getChartActual();
        auto& bufTarget = this_->app.getChartTarget();
        int64_t current = scrollConst*pow(2, this_->scrollZoomLevel)/10;
        LV_LOG_USER("MenuRun | scroll: %ld (off=%ld,%ld)",
            current, bufCurrent.getFileOffset(), bufTarget.getFileOffset());
        bufCurrent.scrollBy(current);
        bufTarget.scrollBy(current);
        if (!this_->updateChart())
        {
            lv_chart_refresh(this_->chart);
        }
    }
}

void MenuRun::onClickScrollPlus(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto& bufCurrent = this_->app.getChartActual();
        auto& bufTarget = this_->app.getChartTarget();
        this_->scrollZoomLevel -= 1;
        if (this_->scrollZoomLevel <= 0)
        {
            this_->scrollZoomLevel = 0;
        }
        LV_LOG_USER("MenuRun | zoom: %lf", pow(2,this_->scrollZoomLevel));
        this_->updateZoom();
    }
}

void MenuRun::onClickScrollMinus(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        this_->scrollZoomLevel += 1;
        if (this_->scrollZoomLevel >= 9)
        {
            this_->scrollZoomLevel = 9;
        }
        LV_LOG_USER("MenuRun | zoom: %lf", pow(2,this_->scrollZoomLevel));
        this_->updateZoom();
    }
}

void MenuRun::onClickScrollFirst(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto& bufCurrent = this_->app.getChartActual();
        auto& bufTarget = this_->app.getChartTarget();
        LV_LOG_USER("MenuRun | scroll: %ld (off=%ld,%ld)",
            bufCurrent.FIRST, bufCurrent.getFileOffset(), bufTarget.getFileOffset());
        bufCurrent.scrollBy(bufCurrent.FIRST);
        bufTarget.scrollBy(bufTarget.FIRST);
        if (!this_->updateChart())
        {
            lv_chart_refresh(this_->chart);
        }
    }
}

void MenuRun::onClickScrollLast(lv_event_t* e)
{
    auto this_   = (MenuRun*) lv_event_get_user_data(e);
    auto element = (Button*) lv_event_get_current_target(e);
    if (e->code == LV_EVENT_CLICKED)
    {
        auto& bufCurrent = this_->app.getChartActual();
        auto& bufTarget = this_->app.getChartTarget();
        LV_LOG_USER("MenuRun | scroll: %ld (off=%ld,%ld)",
            bufCurrent.LAST, bufCurrent.getFileOffset(), bufTarget.getFileOffset());
        bufCurrent.scrollBy(bufCurrent.LAST);
        bufTarget.scrollBy(bufTarget.LAST);
        if (!this_->updateChart())
        {
            lv_chart_refresh(this_->chart);
        }
    }
}

} // namespace ui
