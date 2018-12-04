add_subdirectory(nmea_sentence_reader)

include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/sim)
include_directories(${REPO_ROOT_DIR}/src/fw)

add_library(gps_transaction_recorder
                gps_transaction_recorder.cpp
                gps_transaction.cpp
                gps_transaction_log_entry.cpp
                gps_transaction_serializer.cpp
                nmea_sentence_reader/nmea_sentence_reader.cpp)

target_link_libraries(gps_transaction_recorder
                        transaction_logger)