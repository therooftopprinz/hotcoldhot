cmake_minimum_required(VERSION 3.14)
project("HotColdHot")

SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline-functions -fno-omit-frame-pointer")


set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/main)