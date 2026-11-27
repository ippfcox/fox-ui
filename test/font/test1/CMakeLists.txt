set(TARGET FoxUI-Test-Font-Test1)

list(APPEND SRCS
    test.cpp
)

add_executable(${TARGET} ${SRCS})

target_include_directories(${TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_directories(${TARGET}
    PRIVATE
)

target_link_libraries(${TARGET}
    PRIVATE
    fox
)
