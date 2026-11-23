set(TARGET FoxUI-Playground-Helloworld3)

list(APPEND SRCS
    main.cpp
)

add_executable(${TARGET} ${SRCS})

target_include_directories(${TARGET}
    PRIVATE
)

target_link_directories(${TARGET}
    PRIVATE
)

find_package(X11 REQUIRED)

target_link_libraries(${TARGET}
    PRIVATE
    spdlog::spdlog
    lvgl::lvgl
    freetype
    util
    X11
)
