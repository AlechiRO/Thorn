add_subdirectory(generics)
add_subdirectory(lexer)
add_subdirectory(util)
add_subdirectory(AST)
add_subdirectory(parser)


find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    # Stack test executable
    add_executable (STACK_TEST_DRIVER 
        ${STACK_TEST_SOURCE}
    )
    target_link_libraries(STACK_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(STACK_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # DLL test executable
    add_executable (DLL_TEST_DRIVER
        ${DLL_TEST_SOURCE}
    )
    target_link_libraries(DLL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(DLL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Lexer Util test executable
    add_executable(LEXER_UTIL_TEST_DRIVER
        ${LEXER_UTIL_TEST_SOURCE}
        $<TARGET_OBJECTS:lexer_util_logic>
        $<TARGET_OBJECTS:lexer_logic>
        $<TARGET_OBJECTS:string_util_logic>
    )

    target_link_libraries(LEXER_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(LEXER_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Array List test executable

    add_executable(ARRAY_LIST_TEST_DRIVER
        ${ARRAY_LIST_TEST_SOURCE}
    )

    target_link_libraries(ARRAY_LIST_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(ARRAY_LIST_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # HashMap test executable

    add_executable(HASHMAP_TEST_DRIVER
        ${HASHMAP_TEST_SOURCE}
    )

    target_link_libraries(HASHMAP_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(HASHMAP_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Token and Literal test executable

    add_executable(TOKEN_TEST_DRIVER
        ${TOKEN_TEST_SOURCE}
        $<TARGET_OBJECTS:token_logic>
    )
    
    target_link_libraries(TOKEN_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(TOKEN_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # String_util test executable

    add_executable(STRING_UTIL_TEST_DRIVER
        ${STRING_UTIL_TEST_SOURCE}
        $<TARGET_OBJECTS:string_util_logic>
    )

    target_link_libraries(STRING_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(STRING_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
    )

    # Arena test executable

    add_executable(ARENA_TEST_DRIVER 
        ${ARENA_TEST_SOURCE}
        $<TARGET_OBJECTS:arena_logic>
    )
    target_link_libraries(ARENA_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(ARENA_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
    )

    # Lexer test executable

    add_executable(LEXER_TEST_DRIVER 
        ${LEXER_TEST_SOURCE}
        $<TARGET_OBJECTS:lexer_logic>
        $<TARGET_OBJECTS:lexer_util_logic>
        $<TARGET_OBJECTS:string_util_logic>
        $<TARGET_OBJECTS:printer_logic>
    )
    target_link_libraries(LEXER_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(LEXER_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # AST test executable

    add_executable(AST_TEST_DRIVER 
        ${AST_TEST_SOURCE}
        $<TARGET_OBJECTS:AST_logic>
        $<TARGET_OBJECTS:arena_logic>
        $<TARGET_OBJECTS:token_logic>
    )

    target_link_libraries(AST_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(AST_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Parser_util test executable

    add_executable(PARSER_UTIL_TEST_DRIVER
        ${PARSER_UTIL_TEST_SOURCE}
        $<TARGET_OBJECTS:parser_util_logic>
        $<TARGET_OBJECTS:printer_logic>
        $<TARGET_OBJECTS:arena_logic>
        $<TARGET_OBJECTS:AST_logic>
    )

    target_link_libraries(PARSER_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(PARSER_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Adding Tests
    add_test(NAME test_stack COMMAND STACK_TEST_DRIVER)
    add_test(NAME test_dll COMMAND DLL_TEST_DRIVER)
    add_test(NAME test_lexer_util COMMAND LEXER_UTIL_TEST_DRIVER)
    add_test(NAME test_array_list COMMAND ARRAY_LIST_TEST_DRIVER)
    add_test(NAME test_token COMMAND TOKEN_TEST_DRIVER)
    add_test(NAME test_string_util COMMAND STRING_UTIL_TEST_DRIVER)
    add_test(NAME test_lexer COMMAND LEXER_TEST_DRIVER)
    add_test(NAME test_hashmap COMMAND HASHMAP_TEST_DRIVER)
    add_test(NAME test_arena COMMAND ARENA_TEST_DRIVER)
    add_test(NAME test_AST COMMAND AST_TEST_DRIVER)
    add_test(NAME test_parser_util COMMAND PARSER_UTIL_TEST_DRIVER)

else()
    message(WARNING "CUnit not found!")
endif()