cmake_minimum_required(VERSION 4.0.0)

project(filter VERSION 0.0.1 LANGUAGES C)

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("[Filter] Sanitizers enabled")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/filter.c

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    FILES

    include/filter/filter.h
)
