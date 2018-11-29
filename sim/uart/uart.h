#pragma once

#include <stdlib.h>


class CircularBuffer;

class SerialInterface
{
public:
	virtual ~SerialInterface() {}
	virtual size_t available() const = 0;
	virtual int read() = 0;
	virtual size_t write(char c) = 0;
	size_t write(char *buffer, size_t length);
};


class UartUlator
{
public:
	UartUlator(size_t rx_buffer_size, size_t tx_buffer_size);
	virtual ~UartUlator();

	size_t host_available() const;
	int host_read();
	size_t host_write(char c);

	size_t device_available() const;
	int device_read();
	size_t device_write(char c);

	SerialInterface *get_host_interface();
	SerialInterface *get_device_interface();
private:
	CircularBuffer *_rx_buffer;
	CircularBuffer *_tx_buffer;

	SerialInterface *_host_interface;
	SerialInterface *_device_interface;
};
