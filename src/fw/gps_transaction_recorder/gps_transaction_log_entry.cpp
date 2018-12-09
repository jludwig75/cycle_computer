#include "gps_transaction_log_entry.h"


gps_transaction_log_entry::gps_transaction_log_entry(uint64_t time_stamp, uint8_t transaction_type) :
    transaction_log_entry(time_stamp, GPS_TRANSACTION_OWNER_ID),
    transaction_type(transaction_type)
{
}


nmea_transaction_log_entry::nmea_transaction_log_entry(uint64_t time_stamp, uint8_t sentence_length) :
    gps_transaction_log_entry(time_stamp, GPS_TRANSACTION_TYPE_NMEA),
    sentence_length(sentence_length)
{
}

size_t nmea_transaction_log_entry::entry_bytes() const
{
    return sizeof(*this) + sentence_length;
}


pps_transaction_log_entry::pps_transaction_log_entry(uint64_t time_stamp) :
    gps_transaction_log_entry(time_stamp, GPS_TRANSACTION_TYPE_PPS)
{
}

size_t pps_transaction_log_entry::entry_bytes() const
{
    return sizeof(*this);
}
