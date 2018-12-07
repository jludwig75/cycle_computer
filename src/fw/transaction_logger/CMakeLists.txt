include_directories(~/.platformio/packages/framework-arduinoespressif32/libraries/FS/src)
include_directories(~/.platformio/packages/framework-arduinoespressif32/libraries/SPIFFS/src)
include_directories(${REPO_ROOT_DIR}/src/sim/arduino)



add_library (transaction_logger
                ~/.platformio/packages/framework-arduinoespressif32/libraries/FS/src/FS.cpp
                transaction_logger.cpp
                transaction.cpp
                transaction_log_entry.cpp
                ${REPO_ROOT_DIR}/src/sim/SPIFFS_sim/SPIFFS_sim.cpp # @todo this temporary
                ${REPO_ROOT_DIR}/src/sim/SPIFFS_sim/vfs.cpp) # @todo this temporary