#pragma once

#include <stdint.h>
#include <stdlib.h>


#define GPS_TRANSACTION_OWNER_ID  1


struct transaction_log_entry
{
    transaction_log_entry();
    transaction_log_entry(uint64_t time_stamp, uint8_t transaction_owner);
    virtual size_t entry_bytes() const = 0;
    uint16_t id;
    uint64_t time_stamp;
    uint8_t transaction_owner;
};
