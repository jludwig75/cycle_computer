include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/fw)
include_directories(${REPO_ROOT_DIR}/src/sim)
include_directories(~/.platformio/packages/framework-arduinoespressif32/libraries/FS/src)
include_directories(~/.platformio/packages/framework-arduinoespressif32/libraries/SPIFFS/src)
include_directories(${REPO_ROOT_DIR}/src/sim/arduino)
include_directories(${REPO_ROOT_DIR}/include)

find_package (Threads)

#set_source_files_properties(fw/cycle_transaction_logger.ino PROPERTIES LANGUAGE CXX)

add_executable(cycle_transaction_logger_sim
                fw/cycle_transaction_logger.cpp
                sim/setup_cycle_logger_simulated_hw.cpp
                sim/setup_cycle_logger_simulated_fw.cpp)

target_link_libraries(cycle_transaction_logger_sim
                        interrupt_manager
                        app_sim
                        sim
                        transaction_logger
                        gps_transaction_recorder
                        ${CMAKE_THREAD_LIBS_INIT})