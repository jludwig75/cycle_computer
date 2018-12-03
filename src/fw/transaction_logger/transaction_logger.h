#pragma once

#include "transaction.h"


class TransactionLogger
{
public:
    TransactionLogger(const char *log_file_name);
    /// @brief Writes the transaction to the log. Does not keep the log open after writing
    bool write_transaction(const Transaction &transction);
};