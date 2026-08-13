add_subdirectory(util)

add_library(parser_logic OBJECT parser.c)

target_include_directories(parser_util_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
)