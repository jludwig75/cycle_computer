add_subdirectory(uart_ulator)
add_subdirectory(app)

add_library (sim)

target_link_libraries (sim uart_ulator)