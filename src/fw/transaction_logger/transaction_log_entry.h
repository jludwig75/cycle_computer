#pragma once

#include <stdint.h>
#include <stdlib.h>


#define GPS_TRANSACTION_OWNER_ID  1


struct __attribute__((__packed__)) transaction_log_entry
{
    transaction_log_entry();
    transaction_log_entry(uint64_t time_stamp, uint8_t transaction_owner);
    uint16_t id;
    uint64_t time_stamp;
    uint8_t transaction_owner;
};
