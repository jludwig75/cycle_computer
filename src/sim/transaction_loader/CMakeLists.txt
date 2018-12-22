include_directories(${REPO_ROOT_DIR}/lib/intf)
include_directories(${REPO_ROOT_DIR}/src/sim)
include_directories(${REPO_ROOT_DIR}/src/fw)
include_directories(${REPO_ROOT_DIR}/include)

add_library (transaction_loader transaction_loader.cpp)
