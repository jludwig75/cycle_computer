add_subdirectory(uart_ulator)
add_subdirectory(gps_ulator)
add_subdirectory(mcu)
add_subdirectory(app)

add_library (sim)

target_link_libraries(sim uart_ulator
                        gps_ulator mcu_sim)