cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
    include(${CMAKE_SOURCE_DIR}/main/cmake_idf.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/main/cmake_sim_linux.cmake)
endif()