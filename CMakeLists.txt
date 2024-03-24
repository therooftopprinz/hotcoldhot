cmake_minimum_required(VERSION 3.16)

project(hotcoldhot LANGUAGES C CXX)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

# LVGL CONFIG
set(LV_CONF_PATH ${PROJECT_DIR}/include/lv_conf.h CACHE STRING "" FORCE)
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
add_definitions(-DLV_CONF_PATH=${LV_CONF_PATH})

# DRIVER
set(LVDRV_DIR "${PROJECT_DIR}/sim_components/lvgl-sdl/lvgl_drv")
add_library(lvglsdl ${LVDRV_DIR}/lv_sdl_disp.c ${LVDRV_DIR}/lv_port_indev.c)
target_include_directories(lvglsdl PUBLIC ${SDL2_INCLUDE_DIRS})

# LVGL
set(LVGL_DIR "${PROJECT_DIR}/sim_components/lvgl")
include(${LVGL_DIR}/CMakeLists.txt)

# HOTCOLDHOT
file(GLOB SRC
    "${PROJECT_DIR}/src/App.cpp"
    "${PROJECT_DIR}/src/ui/UI.cpp"
    "${PROJECT_DIR}/src/ui/gl/Button.cpp"
    "${PROJECT_DIR}/src/ui/gl/Keyboard.cpp"
    "${PROJECT_DIR}/src/ui/gl/Label.cpp"
    "${PROJECT_DIR}/src/ui/gl/MessageBox.cpp"
    "${PROJECT_DIR}/src/ui/gl/TabView.cpp"
    "${PROJECT_DIR}/src/ui/gl/TextArea.cpp"
    "${PROJECT_DIR}/src/ui/GlobalStyles.cpp"
    "${PROJECT_DIR}/src/ui/menu/MenuProgram.cpp"
    "${PROJECT_DIR}/src/ui/menu/MenuRun.cpp"
    "${PROJECT_DIR}/src/arduino.cpp"
    "${PROJECT_DIR}/sim_components/main.cpp"
    "${PROJECT_DIR}/sim_components/display_manager/src/display_manager.c"
    "${PROJECT_DIR}/sim_components/storage_manager/src/storage_manager.c"
    "${PROJECT_DIR}/sim_components/board_devices/src/board_devices.cpp")

SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS_DEBUG   "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")

# add_compile_options(-Wall)
add_executable(hotcoldhot ${SRC})

include_directories(${LVDRV_DIR})
include_directories(${LVGL_DIR})
include_directories(${PROJECT_DIR})
include_directories(${PROJECT_DIR}/src)
include_directories(${PROJECT_DIR}/sim_components)
include_directories(${PROJECT_DIR}/lib/display_manager/src)
include_directories(${PROJECT_DIR}/lib/storage_manager/src)
include_directories(${PROJECT_DIR}/lib/board_devices/src)

target_include_directories(hotcoldhot PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(hotcoldhot PUBLIC ${SDL2_CFLAGS_OTHER})
target_link_libraries(hotcoldhot ${SDL2_LIBRARIES} lvglsdl lvgl::lvgl SDL2)
