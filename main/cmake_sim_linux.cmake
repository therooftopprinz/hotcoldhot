project(hotcoldhot LANGUAGES C CXX)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

# LVGL CONFIG
set(LV_CONF_PATH ${PROJECT_DIR}/main/lv_conf.h CACHE STRING "" FORCE)
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
add_definitions(-DLV_CONF_PATH=${LV_CONF_PATH})

# DRIVER
set(LVDRV_DIR "${PROJECT_DIR}/components/lvgl-sdl/lvgl_drv")
add_library(lvglsdl ${LVDRV_DIR}/lv_sdl_disp.c ${LVDRV_DIR}/lv_port_indev.c)
target_include_directories(lvglsdl PUBLIC ${SDL2_INCLUDE_DIRS})

# LVGL
set(LVGL_DIR "${PROJECT_DIR}/components/lvgl")
include(${LVGL_DIR}/CMakeLists.txt)

# HOTCOLDHOT
file(GLOB SRC
    "${PROJECT_DIR}/main/App.cpp"
    "${PROJECT_DIR}/main/ui/UI.cpp"
    "${PROJECT_DIR}/main/ui/gl/Button.cpp"
    "${PROJECT_DIR}/main/ui/gl/Keyboard.cpp"
    "${PROJECT_DIR}/main/ui/gl/Label.cpp"
    "${PROJECT_DIR}/main/ui/gl/MessageBox.cpp"
    "${PROJECT_DIR}/main/ui/gl/TabView.cpp"
    "${PROJECT_DIR}/main/ui/gl/TextArea.cpp"
    "${PROJECT_DIR}/main/ui/GlobalStyles.cpp"
    "${PROJECT_DIR}/main/ui/MenuProgram/MenuProgram.cpp"
    "${PROJECT_DIR}/main/ui/MenuProgram/ProgramBuilder.cpp"
    "${PROJECT_DIR}/main/main.cpp")

add_compile_options(-Wall)
add_executable(hotcoldhot ${SRC})

include_directories(${LVDRV_DIR})
include_directories(${LVGL_DIR})
include_directories(${PROJECT_DIR})

target_include_directories(hotcoldhot PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(hotcoldhot PUBLIC ${SDL2_CFLAGS_OTHER})
target_link_libraries(hotcoldhot ${SDL2_LIBRARIES} lvglsdl lvgl::lvgl SDL2)