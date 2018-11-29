add_subdirectory(circular_buffer)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/circular_buffer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${REPO_ROOT_DIR}/gmock-global/include/)

set(UNIT_TEST_NAME uart_unittest)
cxx_executable(${UNIT_TEST_NAME} test gmock_main uart.cpp)
add_custom_command(
     TARGET ${UNIT_TEST_NAME}
     COMMENT "${UNIT_TEST_NAME} uint tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${UNIT_TEST_NAME}
)

set(UNIT_TEST_NAME uart_circular_buffer_integration_test)
cxx_executable(${UNIT_TEST_NAME} test gmock_main uart.cpp circular_buffer/circular_buffer.cpp)
add_custom_command(
     TARGET ${UNIT_TEST_NAME}
     COMMENT "${UNIT_TEST_NAME} integration tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${UNIT_TEST_NAME}
)

add_library (uart uart.cpp circular_buffer/circular_buffer.cpp)