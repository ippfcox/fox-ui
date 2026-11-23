set(TARGET FoxUI-Playground-Freetype)

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

target_link_libraries(${TARGET}
    PRIVATE
    freetype
)
