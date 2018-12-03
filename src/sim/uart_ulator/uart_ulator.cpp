#include "uart_ulator.h"

#include "circular_buffer/circular_buffer.h"
#include "uart.h"


size_t SerialInterface::write(const char *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (write(buffer[i]) != 1)
        {
            return i;
        }
    }

    return length;
}


class HostSerialInterface : public SerialInterface
{
public:
    HostSerialInterface(UartUlator &uart) :
        _uart(uart)
    {
    }
	virtual size_t available() const
    {
        return _uart.host_available();
    }
	virtual int read()
    {
        return _uart.host_read();
    }
	virtual size_t write(uint8_t c)
    {
        return _uart.host_write(c);
    }
private:
    UartUlator &_uart;
};


class DeviceSerialInterface : public SerialInterface
{
public:
    DeviceSerialInterface(UartUlator &uart) :
        _uart(uart)
    {
    }
	virtual size_t available() const
    {
        return _uart.device_available();
    }
	virtual int read()
    {
        return _uart.device_read();
    }
	virtual size_t write(uint8_t c)
    {
        return _uart.device_write(c);
    }
private:
    UartUlator &_uart;
};


UartUlator::UartUlator(size_t rx_buffer_size, size_t tx_buffer_size) :
	_rx_buffer(NULL),
	_tx_buffer(NULL),
	_host_interface(NULL),
	_device_interface(NULL)
{
    _rx_buffer = new CircularBuffer(rx_buffer_size);
    _tx_buffer = new CircularBuffer(tx_buffer_size);

    _host_interface = new HostSerialInterface(*this);
    _device_interface = new DeviceSerialInterface(*this);
}

UartUlator::~UartUlator()
{
    delete _host_interface;
    delete _device_interface;

    delete _rx_buffer;
    delete _tx_buffer;
}

bool UartUlator::start()
{
    return true;
}

void UartUlator::stop()
{
}


size_t UartUlator::host_available() const
{
    return _rx_buffer->number_of_bytes_in_buffer();
}

int UartUlator::host_read()
{
    char c;
    if (!_rx_buffer->pop(c))
    {
        return -1;
    }

    return (int)c;
}

size_t UartUlator::host_write(char c)
{
    return _tx_buffer->push(c) ? 1 : 0;
}


size_t UartUlator::device_available() const
{
    return _tx_buffer->number_of_bytes_in_buffer();
}

int UartUlator::device_read()
{
    char c;
    if (!_tx_buffer->pop(c))
    {
        return -1;
    }

    return (int)c;
}

size_t UartUlator::device_write(char c)
{
    return _rx_buffer->push(c) ? 1 : 0;
}

SerialInterface *UartUlator::get_host_interface()
{
    return _host_interface;
}

SerialInterface *UartUlator::get_device_interface()
{
    return _device_interface;
}
