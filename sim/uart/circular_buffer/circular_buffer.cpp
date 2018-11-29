#include "circular_buffer.h"

#include <assert.h>


static size_t advance_index(size_t index, size_t max_index)
{
    assert(index < max_index);

    return (index + 1) % max_index;
}

CircularBuffer::CircularBuffer(size_t buffer_size) :
    _buffer_size(buffer_size + 1),
	_buffer(NULL),
	_push_index(0),
	_pop_index(0)
{
    _buffer = new char[buffer_size + 1];
}

bool CircularBuffer::push(char c)
{
    if (advance_index(_push_index, _buffer_size) == _pop_index)
    {
        return false;
    }

    _buffer[_push_index] = c;
    _push_index = advance_index(_push_index, _buffer_size);
    return true;
}

bool CircularBuffer::pop(char &c)
{
    if (_push_index == _pop_index)
    {
        return false;
    }

    c = _buffer[_pop_index];
    _pop_index = advance_index(_pop_index, _buffer_size);
    return true;
}

size_t CircularBuffer::number_of_bytes_in_buffer() const
{
    size_t adjusted_push_index = _push_index;
    if (adjusted_push_index < _pop_index)
    {
        adjusted_push_index += _buffer_size;
    }

    return adjusted_push_index - _pop_index;
}
