cmake_minimum_required(VERSION 3.20)

project(emehcs VERSION 1.0.0)

option(OUTPUT_INTERMEDIATE_OBJ "Outputs intermediate objects." OFF)

set(EMEHCS_SOURCES
    ${CMAKE_SOURCE_DIR}/builtin/add.c
    ${CMAKE_SOURCE_DIR}/builtin/branch.c
    ${CMAKE_SOURCE_DIR}/builtin/comp.c
    ${CMAKE_SOURCE_DIR}/builtin/define.c
    ${CMAKE_SOURCE_DIR}/builtin/display.c
    ${CMAKE_SOURCE_DIR}/builtin/internals.c
    ${CMAKE_SOURCE_DIR}/builtin/lambda.c
    ${CMAKE_SOURCE_DIR}/builtin/let.c
    ${CMAKE_SOURCE_DIR}/builtin/list.c
    ${CMAKE_SOURCE_DIR}/builtin/minus.c
    ${CMAKE_SOURCE_DIR}/builtin/mul.c
    ${CMAKE_SOURCE_DIR}/builtin/pair.c
    ${CMAKE_SOURCE_DIR}/builtin/quote.c
    ${CMAKE_SOURCE_DIR}/builtin/setup.c
    ${CMAKE_SOURCE_DIR}/utils/extarr.c
    ${CMAKE_SOURCE_DIR}/utils/peekable.c
    ${CMAKE_SOURCE_DIR}/env.c
    ${CMAKE_SOURCE_DIR}/evaluation.c
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/optimizer.c
    ${CMAKE_SOURCE_DIR}/parser.c
    ${CMAKE_SOURCE_DIR}/tokenizer.c
)

add_executable(emehcs ${EMEHCS_SOURCES})
target_include_directories(emehcs PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(OUTPUT_INTERMEDIATE_OBJ)
    target_compile_definitions(emehcs PRIVATE EMEHCS_OUTPUT_INTERMEDIATE_OBJ)
endif(OUTPUT_INTERMEDIATE_OBJ)
