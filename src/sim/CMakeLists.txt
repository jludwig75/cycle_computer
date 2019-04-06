add_subdirectory(uart_ulator)
add_subdirectory(gps_ulator)
add_subdirectory(interrupt_ulator)
add_subdirectory(transaction_loader)
add_subdirectory(mcu)
add_subdirectory(app)

add_library (sim dummy.cpp)

target_link_libraries(sim
                        uart_ulator
                        gps_ulator
                        interrupt_ulator
                        mcu_sim
                        transaction_loader)