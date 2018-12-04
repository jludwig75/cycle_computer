#pragma once

#include <stdlib.h>


class NmeaSentenceReader
{
public:
    NmeaSentenceReader(char *sentence_output_buffer, size_t sentence_output_buffer_length);
    virtual ~NmeaSentenceReader();
    bool parse_char(char c);
    void copy_current_line_to_output_buffer();
private:
    char *_sentence_output_buffer;
    char *_current_sentence_buffer;
    const size_t _sentence_output_buffer_length;
    size_t _current_buffer_index;
};