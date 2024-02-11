cmake_minimum_required(VERSION 3.14)

project(hotcoldhot LANGUAGES C CXX)

SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

# LVGL CONFIG
set(LV_CONF_PATH ${PROJECT_DIR}/src/lv_conf.h CACHE STRING "" FORCE)
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
add_definitions(-DLV_CONF_PATH=${LV_CONF_PATH})

# DRIVER
set(LVDRV_DIR "${PROJECT_DIR}/cpp_dependencies/lvgl-sdl/lvgl_drv")
add_library(lvglsdl ${LVDRV_DIR}/lv_sdl_disp.c ${LVDRV_DIR}/lv_port_indev.c)
target_include_directories(lvglsdl PUBLIC ${SDL2_INCLUDE_DIRS})

# LVGL
set(LVGL_DIR "${PROJECT_DIR}/cpp_dependencies/lvgl")
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
    "${PROJECT_DIR}/src/ui/MenuProgram/MenuProgram.cpp"
    "${PROJECT_DIR}/src/ui/MenuProgram/ProgramBuilder.cpp"
    "${PROJECT_DIR}/src/main.cpp")

add_compile_options(-Wall -O2)
add_executable(hotcoldhot ${SRC})

include_directories(${LVDRV_DIR})
include_directories(${LVGL_DIR})
include_directories(${PROJECT_DIR})

target_include_directories(hotcoldhot PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(hotcoldhot PUBLIC ${SDL2_CFLAGS_OTHER})
target_link_libraries(hotcoldhot ${SDL2_LIBRARIES} lvglsdl lvgl::lvgl SDL2)
