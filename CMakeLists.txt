cmake_minimum_required(VERSION 3.16)

project(hotcoldhot LANGUAGES C CXX)

include(ExternalProject)
include(FetchContent)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

# DEBUG FLAGS
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS_DEBUG   "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")

# LVGL CONFIG
set(LV_CONF_PATH ${PROJECT_DIR}/include/lv_conf.h CACHE STRING "" FORCE)
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
add_definitions(-DLV_CONF_PATH=${LV_CONF_PATH})


# LVGL
set(LVGL_DIR "${PROJECT_DIR}/sim_components/lvgl")
include(${LVGL_DIR}/CMakeLists.txt)

# DRIVER
set(LVDRV_DIR "${PROJECT_DIR}/sim_components/lvgl-sdl/lvgl_drv")
add_library(lvglsdl ${LVDRV_DIR}/lv_sdl_disp.c ${LVDRV_DIR}/lv_port_indev.c)
target_include_directories(
    lvglsdl PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${LVGL_DIR})

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
    "${PROJECT_DIR}/src/ui/menu/MenuSettings.cpp"
    "${PROJECT_DIR}/src/arduino.cpp"
    "${PROJECT_DIR}/sim_components/main.cpp"
    "${PROJECT_DIR}/sim_components/display_manager/src/display_manager.c"
    "${PROJECT_DIR}/sim_components/storage_manager/src/storage_manager.c"
    "${PROJECT_DIR}/sim_components/network_manager/src/network_manager.c"
    "${PROJECT_DIR}/lib/board_devices/src/board_devices.cpp")
    
# add_compile_options(-Wall)
add_executable(hotcoldhot ${SRC})

target_include_directories(
    hotcoldhot PUBLIC
        ${LVDRV_DIR}
        ${LVGL_DIR}
        ${PROJECT_DIR}
        ${PROJECT_DIR}/bfc
        ${PROJECT_DIR}/src
        ${PROJECT_DIR}/sim_components
        ${PROJECT_DIR}/lib/display_manager/src
        ${PROJECT_DIR}/lib/storage_manager/src
        ${PROJECT_DIR}/lib/network_manager/src
        ${PROJECT_DIR}/lib/board_devices/src)

target_include_directories(hotcoldhot PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(hotcoldhot PUBLIC ${SDL2_CFLAGS_OTHER})
target_link_libraries(hotcoldhot ${SDL2_LIBRARIES} lvglsdl lvgl::lvgl SDL2)

# TEST
ExternalProject_Add(googletest
  SOURCE_DIR ../sim_components/googletest
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=googletest_install
)

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)
set(GMOCK_INCLUDE_DIRS ${source_dir}/googlemock/include)

ExternalProject_Get_Property(googletest binary_dir)

add_library(gtest UNKNOWN IMPORTED)
set_target_properties(gtest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
    IMPORTED_LOCATION ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
add_dependencies(gtest googletest)

add_library(gtest_main UNKNOWN IMPORTED)
set_target_properties(gtest_main PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a)
add_dependencies(gtest_main googletest)

add_library(gmock UNKNOWN IMPORTED)
set_target_properties(gmock PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${GMOCK_INCLUDE_DIRS}
    IMPORTED_LOCATION ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a)
add_dependencies(gmock googletest)

add_library(gmock_main UNKNOWN IMPORTED)
set_target_properties(gmock_main PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main.a)
add_dependencies(gmock_main gtest)

file(GLOB_RECURSE test_src test/*.cpp)
add_executable(test
    ${test_src})

target_include_directories(test PRIVATE
    src
    test
    ${LVGL_DIR})

target_link_libraries(test PRIVATE pthread gtest gtest_main gmock gmock_main)

# REMOTE SIM
file(GLOB SRC_REMOTE_SIM
    "${PROJECT_DIR}/remote_sim/main.cpp"
    "${PROJECT_DIR}/sim_components/board_devices/src/board_devices.cpp")

# add_compile_options(-Wall)
add_executable(remote_sim ${SRC_REMOTE_SIM})

target_include_directories(
    remote_sim PUBLIC
    ${PROJECT_DIR}
    ${PROJECT_DIR}/bfc
    ${PROJECT_DIR}/remote_sim
    ${PROJECT_DIR}/src
    ${PROJECT_DIR}/lib/board_devices/src)