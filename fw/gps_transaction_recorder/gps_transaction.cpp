#include "uart_transaction.h"

#include <assert.h>


UartTransaction::UartTransaction(uint64_t transaction_time_us, TranactionType type) :
    _transaction_time_us(transaction_time_us),
    _type(type),
    _sentence()
{
    assert(PPS_pulse == _type);
}

UartTransaction::UartTransaction(uint64_t transaction_time_us, TranactionType type, const std::string &nmea_sentence) :
    _transaction_time_us(transaction_time_us),
    _type(type),
    _sentence(nmea_sentence)
{
    assert(NMEA_sentence == _type);
}

uint64_t UartTransaction::transaction_time_us() const
{
    return _transaction_time_us;
}

UartTransaction::TranactionType UartTransaction::transaction_type() const
{
    return _type;
}

std::string UartTransaction::get_nmea_sentence() const
{
    assert(NMEA_sentence == _type);

    return _sentence;
}
