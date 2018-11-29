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

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_MODULE_PATH "${REPO_ROOT_DIR}/cmake/Modules/")
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_LCOV_EXCLUDES '${CMAKE_CURRENT_SOURCE_DIR}/test/*' '${REPO_ROOT_DIR}/googletest/*' '${REPO_ROOT_DIR}/gmock-global/*')
    set(COVERAGE_GCOVR_EXCLUDES ${COVERAGE_LCOV_EXCLUDES})
    SETUP_TARGET_FOR_COVERAGE_LCOV(NAME ${UNIT_TEST_NAME}_coverage EXECUTABLE ${UNIT_TEST_NAME} DEPENDENCIES ${UNIT_TEST_NAME})
endif()