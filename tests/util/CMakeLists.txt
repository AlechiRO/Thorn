add_subdirectory(string_util)
add_subdirectory(data_structures)

# Define the test sources found in subdirectories to the root test CMakesLists.txt file
set(STRING_UTIL_TEST_SOURCE ${STRING_UTIL_TEST_SOURCE} PARENT_SCOPE)
set(ARENA_TEST_SOURCE ${ARENA_TEST_SOURCE} PARENT_SCOPE)