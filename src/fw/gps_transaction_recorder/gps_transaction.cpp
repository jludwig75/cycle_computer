#include "gps_transaction.h"

#include <assert.h>


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
