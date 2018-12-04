#pragma once

#include <stdlib.h>

class SerialInterface;
class TransactionLogger;
class NmeaSentenceReader;


class GpsTransactionRecorder
{
public:
    GpsTransactionRecorder(TransactionLogger &transaction_logger);
    virtual ~GpsTransactionRecorder();
    void begin(SerialInterface *gps_serial_interface);
    void do_work();
private:
    SerialInterface *_gps_serial_interface;
    TransactionLogger &_transaction_logger;
    char *_nmea_parser_output_buffer;
    size_t _nmea_parser_output_buffer_size;
    NmeaSentenceReader *_reader;
};


