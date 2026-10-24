cmake_minimum_required(VERSION 3.31)

project(FoxUI VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "build tests" OFF)
option(BUILD_EXAMPLES "build examples" OFF)
option(BUILD_PLAYGROUND "build playground" OFF)
set(LV_BUILD_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config/lv_conf.h CACHE PATH "path to lv_conf.h")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lvgl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/spdlog)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_PLAYGROUND)
    add_subdirectory(playground)
endif()
