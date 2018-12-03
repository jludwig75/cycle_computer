#pragma once

#include <stdlib.h>

#include "serial_interface.h"

class CircularBuffer;

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
