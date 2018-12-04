#include "transaction_log_entry.h"


#define LOGGER_TRANSACTION_ID   0x5150


transaction_log_entry::transaction_log_entry() :
    time_stamp(0),
    id(0),
    transaction_owner(0)
{
}

transaction_log_entry::transaction_log_entry(uint64_t time_stamp, uint8_t transaction_owner) :
    time_stamp(time_stamp),
    id(LOGGER_TRANSACTION_ID),
    transaction_owner(transaction_owner)
{
}
