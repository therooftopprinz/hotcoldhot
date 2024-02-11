cmake_minimum_required(VERSION 3.5)

project(hot_cold_hot)

if(SIM_LINUX)
    include(${CMAKE_SOURCE_DIR}/main/cmake_sim_linux.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/main/cmake_idf.cmake)
endif()