#pragma once

#include <stdlib.h>

#include "interrupt_manager/InterruptManager.h"

class SerialInterface;
class TransactionLogger;
class NmeaSentenceReader;
class GpsTransactionWriter;


class GpsTransactionRecorder : public InterruptClass
{
public:
    GpsTransactionRecorder(TransactionLogger &transaction_logger);
    virtual ~GpsTransactionRecorder();
    void begin(SerialInterface *gps_serial_interface);
    void do_work();
private:
    virtual void on_high_interrupt() override;
    void read_serial_port();

    SerialInterface *_gps_serial_interface;
    char *_nmea_parser_output_buffer;
    size_t _nmea_parser_output_buffer_size;
    NmeaSentenceReader *_reader;
    GpsTransactionWriter *_writer;
};


