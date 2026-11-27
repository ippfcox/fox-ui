set(TARGET fox)

list(APPEND SRCS
    # core/
    font/font.cpp
    font/font_data.cpp
    font/font_manager.cpp
    font/lvgl_font_backend.cpp
    font/lvgl_font_resource.cpp
)

add_library(${TARGET} ${SRCS})

target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/lvgl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../config
)

