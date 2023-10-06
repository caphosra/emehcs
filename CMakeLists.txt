cmake_minimum_required(VERSION 3.20)

project(emehcs VERSION 1.0.0)

set(EMEHCS_SOURCES
    ${CMAKE_SOURCE_DIR}/utils/extarr.c
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/tokenizer.c
)

add_executable(emehcs ${EMEHCS_SOURCES})
target_include_directories(emehcs PRIVATE ${CMAKE_SOURCE_DIR})
