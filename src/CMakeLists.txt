add_subdirectory(util)
add_subdirectory(engine)

add_executable(shell shell.c  
    $<TARGET_OBJECTS:lexer_logic>
    $<TARGET_OBJECTS:printer_logic>
    $<TARGET_OBJECTS:lexer_util_logic>
    $<TARGET_OBJECTS:string_util_logic>
    $<TARGET_OBJECTS:parser_logic>
    $<TARGET_OBJECTS:parser_util_logic>
    $<TARGET_OBJECTS:AST_logic>
    $<TARGET_OBJECTS:arena_logic>
)


target_include_directories(shell PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules 
    ${CMAKE_SOURCE_DIR}/inc/generics
)