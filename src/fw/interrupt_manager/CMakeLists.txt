include_directories(${REPO_ROOT_DIR}/src/sim/arduino)
include_directories(${REPO_ROOT_DIR}/include)

add_library (interrupt_manager
                InterruptManager.cpp)