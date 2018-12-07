#pragma once

#include "transaction_logger/transaction_log_entry.h"


struct __attribute__((__packed__)) gps_transaction_log_entry : public transaction_log_entry
{
    gps_transaction_log_entry(uint64_t time_stamp, uint8_t transaction_type);
    uint8_t transaction_type;
};

struct __attribute__((__packed__)) nmea_transaction_log_entry : public gps_transaction_log_entry
{
    nmea_transaction_log_entry(uint64_t time_stamp, uint8_t sentence_length);
    size_t entry_bytes() const;
    uint8_t sentence_length;
};

struct __attribute__((__packed__)) pps_transaction_log_entry : public gps_transaction_log_entry
{
    pps_transaction_log_entry(uint64_t time_stamp);
    size_t entry_bytes() const;
};
