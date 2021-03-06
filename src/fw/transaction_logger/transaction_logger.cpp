#include "transaction_logger.h"

#include <FS.h>
#include <SPIFFS.h>



TransactionLogger::TransactionLogger(const char *log_file_name) :
    _log_file_name(log_file_name)
{
    
}

bool TransactionLogger::write_transaction(const Transaction &transaction)
{
    uint8_t transaction_blob[MAX_TRANSACTION_BLOB_BYTES];

    size_t transaction_blob_bytes_written;
    if (!transaction.generate_transaction_blob(transaction_blob, sizeof(transaction_blob), transaction_blob_bytes_written))
    {
        return false;
    }

    /// @todo Open the transaction log file for append.
    File file = SPIFFS.open(_log_file_name, FILE_APPEND);
    if(!file){
        //Serial.println("- failed to open file for appending");
        return false;
    }

    /// @todo Write the transaction the the log file.
    if (file.write(transaction_blob, transaction_blob_bytes_written) != transaction_blob_bytes_written)
    {
        return false;
    }

    /// @todo Close the transaction log file.
    file.close();

    return true;
}
