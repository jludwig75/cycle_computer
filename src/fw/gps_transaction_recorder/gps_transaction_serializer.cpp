#include "gps_transaction_serializer.h"

#include "gps_transaction_log_entry.h"

#include <string.h>


bool GpsTransactionSerializer::serialize_nmea_sentence(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time,
                                const char *sentence,
                                size_t sentence_length)
{
    nmea_transaction_log_entry entry(transaction_time, sentence_length);
    if (transaction_stream_bytes_remaining < entry.entry_bytes())
    {
        return false;
    }
    bytes_written_to_stream = entry.entry_bytes();
    memcpy(&transaction_stream[0], &entry, sizeof(entry));
    memcpy(&transaction_stream[sizeof(entry)], sentence, sentence_length);
    return true;
}

bool GpsTransactionSerializer::serialize_pps_pulse(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time)
{
    pps_transaction_log_entry entry(transaction_time);
    if (transaction_stream_bytes_remaining < entry.entry_bytes())
    {
        return false;
    }
    bytes_written_to_stream = entry.entry_bytes();
    memcpy(&transaction_stream[0], &entry, sizeof(entry));
    return true;
}

bool GpsTransactionSerializer::desrialize_uart_transaction(uint8_t *transaction_stream,
                                    size_t transaction_stream_bytes_remaining,
                                    size_t &bytes_read_from_stream,
                                    GpsTransaction &deserialized_transaction)
{
    if (transaction_stream_bytes_remaining < sizeof(gps_transaction_log_entry))
    {
        return false;
    }

    gps_transaction_log_entry *gps_entry = (gps_transaction_log_entry *)transaction_stream;
    if (gps_entry->transaction_owner != GPS_TRANSACTION_OWNER_ID)
    {
        return false;
    }

    switch(gps_entry->transaction_type)
    {
    case GPS_TRANSACTION_TYPE_NMEA:
    {
        if (transaction_stream_bytes_remaining < sizeof(nmea_transaction_log_entry))
        {
            return false;
        }
        nmea_transaction_log_entry *nmea_entry = (nmea_transaction_log_entry*)transaction_stream;
        if (nmea_entry->sentence_length == 0)
        {
            /// @todo Maybe there is a minimum NMEA length.
            return false;
        }
        /// @todo Maybe compare the length to a max NMEA length.
        size_t total_entry_bytes = sizeof(*nmea_entry) + nmea_entry->sentence_length;
        if (transaction_stream_bytes_remaining < total_entry_bytes)
        {
            return false;
        }

        std::string nmea_sentence((const char *)&transaction_stream[sizeof(*nmea_entry)], nmea_entry->sentence_length);

        bytes_read_from_stream = total_entry_bytes;
        deserialized_transaction = GpsTransaction(nmea_entry->time_stamp, GpsTransaction::NMEA_sentence, nmea_sentence);
        return true;
    }
    case GPS_TRANSACTION_TYPE_PPS:
    {
        pps_transaction_log_entry *nmea_entry = (pps_transaction_log_entry*)transaction_stream;

        bytes_read_from_stream = sizeof(*nmea_entry);
        deserialized_transaction = GpsTransaction(nmea_entry->time_stamp, GpsTransaction::PPS_pulse);
        return true;
    }
    default:
        return false;
    }
}

GpsTransactionSerializer GpsSerializer;