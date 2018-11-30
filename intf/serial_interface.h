#pragma once

#include <stdlib.h>


class SerialInterface
{
public:
	virtual ~SerialInterface() {}
	virtual size_t available() const = 0;
	virtual int read() = 0;
	virtual size_t write(char c) = 0;
	size_t write(const char *buffer, size_t length);
};
