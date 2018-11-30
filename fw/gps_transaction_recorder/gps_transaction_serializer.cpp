#include "uart_transaction_serializer.h"


bool UartTransactionSerializer::serialize_nmea_sentence(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time,
                                const char *sentence)
{

}

bool UartTransactionSerializer::serialize_pps_pulse(uint8_t *transaction_stream,
                                size_t transaction_stream_bytes_remaining,
                                size_t &bytes_written_to_stream,
                                uint64_t transaction_time)
{

}

bool UartTransactionSerializer::desrialize_uart_transaction(uint8_t *transaction_stream,
                                    size_t transaction_stream_bytes_remaining,
                                    size_t &bytes_read_from_stream,
                                    UartTransaction &deserialized_transaction)
{

}


