#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "uart_transatcion.h"

class UartTransacionList
{
public:
    size_t deserialize_transaction(uint8_t *transaction_stream, size_t transaction_stream_bytes_remaining);
    UartTransaction pop();
};