#include "uart_ulator/uart_ulator.h"
#include "gps_ulator/gps_ulator.h"
#include "interrupt_ulator/interrupt_ulator.h"
#include "uart.h"

#include <assert.h>

UartUlator uart0(1024, 1024);
UartUlator uart1(1024, 1024);
UartUlator uart2(1024, 1024);

class Esp32 : public Ulator
{
public:
    Esp32(UartUlator &uart0, UartUlator &uart1, UartUlator &uart2) : 
        _uart0(uart0),
        _uart1(uart1),
        _uart2(uart2)
    {

    }
    bool start()
    {
        if (!_uart0.start() || !_uart1.start() || !_uart2.start() || !InterruptUlator.start())
        {
            stop();
            return false;
        }

        return true;
    }
    void stop()
    {
        _uart0.stop();
        _uart1.stop();
        _uart2.stop();
        InterruptUlator.stop();
    }

    UartUlator & get_uart0()
    {
        return _uart0;
    }
    UartUlator & get_uart1()
    {
        return _uart1;
    }
    UartUlator & get_uart2()
    {
        return _uart2;
    }

private:
    UartUlator &_uart0;
    UartUlator &_uart1;
    UartUlator &_uart2;
} esp32(uart0, uart1, uart2);

class Lolin32 : public Ulator
{
public:
    Lolin32(Esp32 &esp32) :
        _esp32(esp32)
    {

    }
    Esp32 &_esp32;
    bool start()
    {
        return _esp32.start();
    }
    void stop()
    {
        _esp32.stop();
    }
    UartUlator & get_uart0()
    {
        return _esp32.get_uart0();
    }
    UartUlator & get_uart1()
    {
        return _esp32.get_uart1();
    }
    UartUlator & get_uart2()
    {
        return _esp32.get_uart2();
    }

};

class CycleLoggerBoard : public Ulator
{
public:
    CycleLoggerBoard() :
        _lolin32(esp32),
        // Attach the GPS to UART1's device serial interface
        _gps(_lolin32.get_uart1().get_device_interface())
    {

    }
    bool start()
    {
        if (!_lolin32.start() || !_gps.start())
        {
            stop();
            return false;
        }
        return true;
    }
    void stop()
    {
        _gps.stop();
        _lolin32.stop();
    }

    SerialInterface *uart__get_serial_interface(size_t uart_number)
    {
        switch(uart_number)
        {
        case 0:
            return uart0.get_host_interface();
        case 1:
            return uart1.get_host_interface();
        case 2:
            return uart2.get_host_interface();
        default:
            return NULL;
        }
    }

    Lolin32 _lolin32;
    GpsUlator _gps;
} board;

SerialInterface *uart__get_serial_interface(size_t uart_number)
{
    return board.uart__get_serial_interface(uart_number);
}


void setup_app_hw_sim()
{
    bool ret = board.start();
    assert(ret);
}