#include <gtest/gtest.h>

#include "gps_transaction_recorder/gps_transaction_serializer.h"
#include "gps_transaction_recorder/gps_transaction_log_entry.h"
#include "gps_transaction_recorder/gps_transaction.h"



TEST(GpsTransactionSerializer, fails_to_serialize_nmea_transaction_if_buffer_too_small)
{
    const std::string nmea_sentence("$NMEA suff\n");
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(nmea_transaction_log_entry) + 2];  // Enough room for the header, but not with this sentence
    size_t bytes_written;
    ASSERT_FALSE(serializer.serialize_nmea_sentence(stream_buffer,
                                        sizeof(stream_buffer),
                                        bytes_written,
                                        test_time,
                                        nmea_sentence.c_str(),
                                        nmea_sentence.length()));
}

TEST(GpsTransactionSerializer, serializes_nmea_transaction)
{
    const std::string nmea_sentence("$NMEA suff\n");
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[100];
    size_t bytes_written;
    ASSERT_TRUE(serializer.serialize_nmea_sentence(stream_buffer,
                                        sizeof(stream_buffer),
                                        bytes_written,
                                        test_time,
                                        nmea_sentence.c_str(),
                                        nmea_sentence.length()));

    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    ASSERT_EQ(test_time, entry->time_stamp);
    ASSERT_EQ(GPS_TRANSACTION_OWNER_ID, entry->transaction_owner);
    ASSERT_EQ(GPS_TRANSACTION_TYPE_NMEA, entry->transaction_type);
    ASSERT_EQ(nmea_sentence.length(), entry->sentence_length);
    ASSERT_TRUE(memcmp(nmea_sentence.c_str(), &stream_buffer[sizeof(*entry)], entry->sentence_length) == 0);
    ASSERT_EQ(sizeof(*entry) + nmea_sentence.length(), bytes_written);
}

TEST(GpsTransactionSerializer, fails_to_serialize_pps_transaction_if_buffer_too_small)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(pps_transaction_log_entry) - 1];   // Not enough room for the header
    size_t bytes_written;
    ASSERT_FALSE(serializer.serialize_pps_pulse(stream_buffer,
                                        sizeof(stream_buffer),
                                        bytes_written,
                                        test_time));
}

TEST(GpsTransactionSerializer, serializes_pps_transaction)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[100];
    size_t bytes_written;
    ASSERT_TRUE(serializer.serialize_pps_pulse(stream_buffer,
                                        sizeof(stream_buffer),
                                        bytes_written,
                                        test_time));

    pps_transaction_log_entry *entry = (pps_transaction_log_entry *)stream_buffer;

    ASSERT_EQ(test_time, entry->time_stamp);
    ASSERT_EQ(GPS_TRANSACTION_OWNER_ID, entry->transaction_owner);
    ASSERT_EQ(GPS_TRANSACTION_TYPE_PPS, entry->transaction_type);
    ASSERT_EQ(sizeof(*entry), bytes_written);
}

TEST(GpsTransactionSerializer, fails_to_deserialize_from_too_small_buffer)
{
    GpsTransactionSerializer serializer;

    uint8_t stream_buffer[3];
    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, fails_to_deserialize_if_owner_id_is_not_gps)
{
    GpsTransactionSerializer serializer;

    uint8_t stream_buffer[100];
    pps_transaction_log_entry *entry = (pps_transaction_log_entry *)stream_buffer;

    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID + 1;


    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, fails_to_deserialize_nmea_sentence_from_too_small_buffer_for_nmea_header)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(nmea_transaction_log_entry) - 1];
    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_NMEA;
    entry->sentence_length = 40;

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, fails_to_deserialize_invalid_transaction_type)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(nmea_transaction_log_entry)];
    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_NMEA + 10;
    entry->sentence_length = 40;

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, fails_to_deserialize_nmea_sentence_from_too_small_buffer_for_sentence)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(nmea_transaction_log_entry)];
    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_NMEA;
    entry->sentence_length = 40;

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, fails_to_deserialize_zero_length_nmea_sentence)
{
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[sizeof(nmea_transaction_log_entry)];
    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_NMEA;
    entry->sentence_length = 0;

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_FALSE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
}

TEST(GpsTransactionSerializer, deserializes_nmea_sentence)
{
    const std::string nmea_sentence("$NMEA suff\n");
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[100];
    nmea_transaction_log_entry *entry = (nmea_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_NMEA;
    entry->sentence_length = nmea_sentence.length();
    memcpy(&stream_buffer[sizeof(*entry)], nmea_sentence.c_str(), entry->sentence_length);

    size_t bytes_expected_to_be_deserialized = sizeof(*entry) + nmea_sentence.length();

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_TRUE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
    ASSERT_EQ(bytes_expected_to_be_deserialized, bytes_deserialized);

    ASSERT_EQ(test_time, deserialized_transaction.transaction_time_us());
    ASSERT_EQ(GpsTransaction::NMEA_sentence, deserialized_transaction.transaction_type());
    ASSERT_EQ(nmea_sentence, deserialized_transaction.get_nmea_sentence());
}

TEST(GpsTransactionSerializer, deserializes_pps_pulse)
{
    const std::string nmea_sentence("$NMEA suff\n");
    const uint64_t test_time = 23454;
    GpsTransactionSerializer serializer;
    
    uint8_t stream_buffer[100];
    pps_transaction_log_entry *entry = (pps_transaction_log_entry *)stream_buffer;

    entry->id = 0x5150;
    entry->time_stamp = test_time;
    entry->transaction_owner = GPS_TRANSACTION_OWNER_ID;
    entry->transaction_type = GPS_TRANSACTION_TYPE_PPS;

    size_t bytes_expected_to_be_deserialized = sizeof(*entry);

    GpsTransaction deserialized_transaction;
    size_t bytes_deserialized;
    ASSERT_TRUE(serializer.desrialize_uart_transaction(stream_buffer,
                                                        sizeof(stream_buffer),
                                                        bytes_deserialized,
                                                        deserialized_transaction));
    ASSERT_EQ(bytes_expected_to_be_deserialized, bytes_deserialized);

    ASSERT_EQ(test_time, deserialized_transaction.transaction_time_us());
    ASSERT_EQ(GpsTransaction::PPS_pulse, deserialized_transaction.transaction_type());
}