#include <gtest/gtest.h>
#include <lvgl.h>


// lvgl linkables
void _lv_log_add(lv_log_level_t level, const char * file, int line,
                 const char * func, const char * format, ...)
{}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}