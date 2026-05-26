cmake_minimum_required(VERSION 4.0.0)

project(app VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("[App] Sanitizers enabled")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
    src/point.c
    src/sequence.c
    src/config.c
    src/main.c

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    FILES

    include/point.h
    include/sequence.h
    include/config.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE filter)
