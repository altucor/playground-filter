cmake_minimum_required(VERSION 4.0.0)

project(filter VERSION 0.0.1 LANGUAGES C)

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/**.c")

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_sources(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SOURCES})

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
