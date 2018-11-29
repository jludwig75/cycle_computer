cmake_minimum_required(VERSION 2.6.4)

set(REPO_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_CXX_STANDARD 11)

project (cycle_computer)

add_subdirectory(googletest)
add_subdirectory(sim)