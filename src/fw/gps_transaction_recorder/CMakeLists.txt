add_subdirectory(nmea_sentence_reader)

include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/sim)
include_directories(${REPO_ROOT_DIR}/src/fw)
include_directories(${REPO_ROOT_DIR}/include)

add_library(gps_transaction_recorder
                gps_transaction_recorder.cpp
                gps_transaction.cpp
                gps_transaction_log_entry.cpp
                gps_transaction_serializer.cpp
                nmea_sentence_reader/nmea_sentence_reader.cpp)

target_link_libraries(gps_transaction_recorder
                        transaction_logger
                        interrupt_manager)

set(UNIT_TEST_NAME gps_transaction_serializer_unittest)
cxx_executable(${UNIT_TEST_NAME}
                test
                gtest_main
                gps_transaction_serializer.cpp
                gps_transaction_log_entry.cpp
                gps_transaction.cpp
                ${REPO_ROOT_DIR}/src/fw/transaction_logger/transaction_log_entry.cpp)
                
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
