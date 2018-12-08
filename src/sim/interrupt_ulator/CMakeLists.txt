include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${REPO_ROOT_DIR}/gmock-global/include/)
include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/sim)
include_directories(${REPO_ROOT_DIR}/src/fw)
include_directories(${REPO_ROOT_DIR}/include)

add_library (interrupt_ulator interrupt_ulator.cpp)