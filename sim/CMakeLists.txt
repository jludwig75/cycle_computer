add_subdirectory(uart)

add_library (sim)

target_link_libraries (sim uart)