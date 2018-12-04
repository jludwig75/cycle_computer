#pragma once

#include <string>

#include <stdint.h>


class GpsTransaction
{
public:
    enum TranactionType { NMEA_sentence, PPS_pulse };
    // PPS constructor
    // type must be PPS_pulse
    GpsTransaction(uint64_t transaction_time_us, TranactionType type);
    // NMEA sentence constructor
    // type must be NMEA_sentence
    GpsTransaction(uint64_t transaction_time_us, TranactionType type, const std::string &nmea_sentence);
    uint64_t transaction_time_us() const; 
    TranactionType transaction_type() const;
    const std::string &get_nmea_sentence() const;
private:
    uint64_t _transaction_time_us;
    TranactionType _type;
    std::string _sentence;
};