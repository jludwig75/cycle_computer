include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(UNIT_TEST_NAME circular_buffer_unittest)

cxx_executable(${UNIT_TEST_NAME} test gtest_main circular_buffer.cpp)

add_custom_command(
     TARGET ${UNIT_TEST_NAME}
     COMMENT "${UNIT_TEST_NAME} uint tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${UNIT_TEST_NAME}
)