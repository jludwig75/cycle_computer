#include "nmea_sentence_reader.h"

#include <assert.h>
#include <string.h>


NmeaSentenceReader::NmeaSentenceReader(char *sentence_output_buffer, size_t sentence_output_buffer_length) :
    _sentence_output_buffer(sentence_output_buffer),
    _current_sentence_buffer(NULL),
    _sentence_output_buffer_length(sentence_output_buffer_length),
    _current_buffer_index(0)
{
    _current_sentence_buffer = new char[sentence_output_buffer_length];
}

NmeaSentenceReader::~NmeaSentenceReader()
{
    delete [] _current_sentence_buffer;
}

bool NmeaSentenceReader::parse_char(char c)
{
    assert(_current_buffer_index < _sentence_output_buffer_length - 1);
    bool sentence_complete = false;

    if (c == '$' && _current_buffer_index > 0)
    {
        copy_current_line_to_output_buffer();
        sentence_complete = true;
    }

    _current_sentence_buffer[_current_buffer_index++] = c;

    if (c == '\r' || c == '\n' || _current_buffer_index == _sentence_output_buffer_length - 1)
    {
        copy_current_line_to_output_buffer();
        sentence_complete = true;
    }

    return sentence_complete;
}

void NmeaSentenceReader::copy_current_line_to_output_buffer()
{
    if (_current_buffer_index > 0)
    {
        memcpy(_sentence_output_buffer, _current_sentence_buffer, _current_buffer_index);
    }
    _sentence_output_buffer[_current_buffer_index] = 0;
    _current_buffer_index = 0;
}
