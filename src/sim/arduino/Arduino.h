#pragma once

#include <string>
#include <stdlib.h>


typedef bool boolean;
typedef std::string String;


class Stream
{
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size)
    {
        for(size_t i = 0; i < size; i++)
        {
            if (write(buffer[i]) == 0)
            {
                return i;
            }
        }

        return size;
    }
protected:
    unsigned long _timeout;      // number of milliseconds to wait for the next char before aborting timed read
    unsigned long _startMillis;  // used for timeout measurement
};