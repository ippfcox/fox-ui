cmake_minimum_required(VERSION 3.31)

project(FoxUI VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "build tests" OFF)
option(BUILD_EXAMPLES "build examples" OFF)
option(BUILD_PLAYGROUND "build playground" OFF)
set(FOXUI_LV_CONF_PATH ${CMAKE_SOURCE_DIR}/config/lv_conf.h CACHE FILEPATH "path to lv_conf.h")

set(LV_BUILD_CONF_PATH ${FOXUI_LV_CONF_PATH} CACHE FILEPATH "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/lvgl)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/spdlog)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# [debug]
set(BUILD_PLAYGROUND true)
if(BUILD_PLAYGROUND)
    add_subdirectory(playground)
endif()
