#pragma once

#include <stdint.h>
#include <stdlib.h>


#define MAX_TRANSACTION_BLOB_BYTES 24


class Transaction
{
public:    
    /// @param  transaction_blob_buffer The buffer to write the transaction blob to
    /// @return The number of bytes written to the transaction blob buffer
    virtual bool generate_transaction_blob(uint8_t *transaction_blob_buffer, size_t transaction_blob_buffer_bytes, size_t &transaction_blob_bytes_written) = 0;
};
