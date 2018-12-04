#include "uart_ulator/uart_ulator.h"
#include "gps_ulator/gps_ulator.h"

#include <assert.h>


class Esp32 : public Ulator
{
public:
    Esp32() : 
        _uart0(1024, 1024),
        _uart1(1024, 1024),
        _uart2(1024, 1024)
    {

    }
    bool start()
    {
        if (!_uart0.start() || !_uart1.start() || !_uart2.start())
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
    UartUlator _uart0;
    UartUlator _uart1;
    UartUlator _uart2;
};

class Lolin32 : public Ulator
{
public:
    Esp32 _esp32;
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
        _lolin32(),
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
    Lolin32 _lolin32;
    GpsUlator _gps;
} board;



void setup_app_hw_sim()
{
    bool ret = board.start();
    assert(ret);
}