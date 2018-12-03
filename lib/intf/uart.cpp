#include "uart.h"

#include "serial_interface.h"

#include <Arduino.h>


class ArduinoSerial : public SerialInterface
{
public:
    ArduinoSerial(HardwareSerial & serial) :
        _serial(serial)
    {

    }
	size_t available() const
    {
        return _serial.available();
    }
	int read()
    {
        return _serial.read();
    }
	size_t write(uint8_t c)
    {
        return _serial.write(c);
    }
private:
    HardwareSerial & _serial;
};

static ArduinoSerial serial0(Serial);
static ArduinoSerial serial1(Serial1);
static ArduinoSerial serial2(Serial2);


SerialInterface *uart__get_serial_interface(size_t uart_number)
{
    switch(uart_number)
    {
        case 0:
            return &serial0;
        case 1:
            return &serial1;
        case 2:
            return &serial2;
        default:
            return NULL;
    }
}