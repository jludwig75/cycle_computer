#pragma once

#include <map>
#include <string>

#include <stdlib.h>
#include <stdint.h>


class TransactionHandler
{
public:
    ~TransactionHandler() {}

    virtual bool load_transaction(uint8_t *transaction_stream, size_t transaction_stream_bytes, size_t & bytes_consumed) = 0;
};

class TransactionLoader
{
public:
    bool register_transaction_handler(uint8_t transaction_owner_id, TransactionHandler *handler);

    bool load_transactions(const std::string & transaction_log_file_name);
private:
    std::map<uint8_t, TransactionHandler *> _transaction_handler_map;
};