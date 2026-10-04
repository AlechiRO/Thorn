add_subdirectory(data_structures)
add_subdirectory(string)
add_subdirectory(memory_management)


target_include_directories(arena_logic PRIVATE
    ${CMAKE_SOURCE_DIR}/inc/modules
)