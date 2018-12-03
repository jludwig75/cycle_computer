#include "uart.h"

#include "uart_ulator.h"

UartUlator serial1(1024, 1024);

SerialInterface *uart__get_serial_interface(size_t uart_number)
{
    return serial1.get_host_interface();
}