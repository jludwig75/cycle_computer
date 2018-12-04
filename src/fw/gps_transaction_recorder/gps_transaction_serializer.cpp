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
    return false;
}

GpsTransactionSerializer GpsSerializer;