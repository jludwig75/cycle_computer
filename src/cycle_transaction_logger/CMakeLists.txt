include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/fw)
include_directories(${REPO_ROOT_DIR}/src/sim)

#set_source_files_properties(fw/cycle_transaction_logger.ino PROPERTIES LANGUAGE CXX)

add_executable(cycle_transaction_logger_sim fw/cycle_transaction_logger.cpp sim/seup_cycle_logger_simulated_hw.cpp)

target_link_libraries(cycle_transaction_logger_sim app_sim transaction_logger gps_transaction_recorder sim)