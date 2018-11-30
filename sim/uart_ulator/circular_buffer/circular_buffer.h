#pragma once


#include <stdlib.h>


class CircularBuffer
{
public:
	CircularBuffer(size_t buffer_size);
	bool push(char c);
	bool pop(char &c);
	size_t number_of_bytes_in_buffer() const;
private:
	size_t _buffer_size;
	char *_buffer;
	size_t _push_index;
	size_t _pop_index;
};
