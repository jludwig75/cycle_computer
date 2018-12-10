#include "gps_transaction.h"

#include "gps_transaction_serializer.h"

#include <assert.h>
#include <string.h>

GpsTransaction::GpsTransaction() :
    _transaction_time_us(0),
    _type(Invalid_transaction_type),
    _sentence()
{
}


GpsTransaction::GpsTransaction(uint64_t transaction_time_us, TranactionType type) :
    _transaction_time_us(transaction_time_us),
    _type(type),
    _sentence()
{
    assert(PPS_pulse == _type);
}

GpsTransaction::GpsTransaction(uint64_t transaction_time_us, TranactionType type, const std::string &nmea_sentence) :
    _transaction_time_us(transaction_time_us),
    _type(type),
    _sentence(nmea_sentence)
{
    assert(NMEA_sentence == _type);
}

uint64_t GpsTransaction::transaction_time_us() const
{
    return _transaction_time_us;
}

GpsTransaction::TranactionType GpsTransaction::transaction_type() const
{
    return _type;
}

const std::string &GpsTransaction::get_nmea_sentence() const
{
    assert(NMEA_sentence == _type);

    return _sentence;
}

bool GpsTransaction::generate_transaction_blob(uint8_t *transaction_blob_buffer, size_t transaction_blob_buffer_bytes, size_t &transaction_blob_bytes_written) const
{
    switch(_type)
    {
    case NMEA_sentence:
        {
            return GpsSerializer.serialize_nmea_sentence(transaction_blob_buffer,
                                                            transaction_blob_buffer_bytes,
                                                            transaction_blob_bytes_written,
                                                            _transaction_time_us,
                                                            _sentence.c_str(),
                                                            _sentence.length());
        }
    case PPS_pulse:
        {
            return GpsSerializer.serialize_pps_pulse(transaction_blob_buffer,
                                                            transaction_blob_buffer_bytes,
                                                            transaction_blob_bytes_written,
                                                            _transaction_time_us);
        }
    default:
        return false;
    }
}
