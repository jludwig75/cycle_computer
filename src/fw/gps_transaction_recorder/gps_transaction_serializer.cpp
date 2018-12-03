#include "gps_transaction_serializer.h"


bool GpsTransactionSerializer::serialize_nmea_sentence(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time,
                                const char *sentence)
{
    return false;
}

bool GpsTransactionSerializer::serialize_pps_pulse(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time)
{
    return false;
}

bool GpsTransactionSerializer::desrialize_uart_transaction(uint8_t *transaction_stream,
                                    size_t transaction_stream_bytes_remaining,
                                    size_t &bytes_read_from_stream,
                                    GpsTransaction &deserialized_transaction)
{
    return false;
}


