#include "transaction_loader.h"
#include "transaction_logger/transaction_log_entry.h"

#include <vector>
#include <cinttypes>

#include <stdio.h>
#include <assert.h>


class CloseFilePointerOnExit
{
public:
    CloseFilePointerOnExit(FILE* &fp) :
        _fp(fp)
    {

    }
    ~CloseFilePointerOnExit()
    {
        if (_fp)
        {
            fclose(_fp);
            _fp = NULL;
        }
    }
private:
    FILE* &_fp;
};


bool TransactionLoader::register_transaction_handler(uint8_t transaction_owner_id, TransactionHandler *handler)
{
    std::map<uint8_t, TransactionHandler *>::iterator i = _transaction_handler_map.find(transaction_owner_id);
    if (i != _transaction_handler_map.end())
    {
        // Transaction handler alredy registered for this owner ID.
        return false;
    }

    _transaction_handler_map[transaction_owner_id] = handler;
    return true;
}

bool TransactionLoader::load_transactions(const std::string & transaction_log_file_name)
{
    FILE *fp = fopen(transaction_log_file_name.c_str(), "r");
    if (!fp)
    {
        fprintf(stderr, "Could not load transaction log file \"%s\"\n.", transaction_log_file_name.c_str());
        return false;
    }
    CloseFilePointerOnExit close_on_exit(fp);

    int ret = fseek(fp, 0, SEEK_END);
    if (ret != 0)
    {
        fprintf(stderr, "Could get size of transaction log file \"%s\"\n.", transaction_log_file_name.c_str());
        return false;
    }
    long pos = ftell(fp);
    if (pos == -1)
    {
        fprintf(stderr, "Could get size of transaction log file \"%s\"\n.", transaction_log_file_name.c_str());
        return false;
    }
    size_t transaction_log_bytes = (size_t)pos;
    ret = fseek(fp, 0, SEEK_SET);
    if (ret != 0)
    {
        fprintf(stderr, "Could get size of transaction log file \"%s\"\n.", transaction_log_file_name.c_str());
        return false;
    }

    std::vector<uint8_t> transaction_log_buffer(transaction_log_bytes, 0);
    if (fread(&transaction_log_buffer[0], 1, transaction_log_bytes, fp) != transaction_log_bytes)
    {
        fprintf(stderr, "Error reading transactions from transaction log file \"%s\"\n.", transaction_log_file_name.c_str());
        return false;
    }

    size_t current_log_position = 0;
    while(current_log_position < transaction_log_buffer.size())
    {
        size_t bytes_remaining_in_log = transaction_log_buffer.size() - current_log_position;
        if (bytes_remaining_in_log < sizeof(transaction_log_entry))
        {
            fprintf(stderr, "Unexected remaining log file size %" PRIu64 "\n", bytes_remaining_in_log);
            return false;
        }
        transaction_log_entry *entry = (transaction_log_entry *)&transaction_log_buffer[current_log_position];

        std::map<uint8_t, TransactionHandler *>::iterator i = _transaction_handler_map.find(entry->id);
        if (i == _transaction_handler_map.end())
        {
            fprintf(stderr, "Found unsupported transaction owner ID %u\n", entry->id);
            return false;
        }

        size_t bytes_consumed;
        if (!i->second->load_transaction(&transaction_log_buffer[current_log_position], bytes_remaining_in_log, bytes_consumed))
        {
            fprintf(stderr, "Failed to load transaction for owner ID %u\n", entry->id);
            return false;
        }
        assert(bytes_consumed <= bytes_remaining_in_log);
        bytes_remaining_in_log -= bytes_consumed;
    }

    return true;
}
