#include "gps_transaction_recorder.h"

#include "mcu_time.h"
#include "serial_interface.h"
#include "nmea_sentence_reader/nmea_sentence_reader.h"
#include "transaction_logger/transaction_logger.h"
#include "gps_transaction.h"
#include "gps_transaction_writer.h"

#include <assert.h>

GpsTransactionRecorder::GpsTransactionRecorder(TransactionLogger &transaction_logger) :
    _gps_serial_interface(NULL),
    _nmea_parser_output_buffer(NULL),
    _nmea_parser_output_buffer_size(20),    /// @todo get a max size from the reader.
    _reader(NULL),
    _writer(NULL)
{
    _nmea_parser_output_buffer = new char[_nmea_parser_output_buffer_size + 1];
    _reader = new NmeaSentenceReader(_nmea_parser_output_buffer, _nmea_parser_output_buffer_size);
    _writer = new GpsTransactionWriter(transaction_logger);
}

void GpsTransactionRecorder::begin(SerialInterface *gps_serial_interface)
{
    _gps_serial_interface = gps_serial_interface;

    InterruptManager.attachInterrupt(25, this, &InterruptClass::on_high_interrupt, InterruptMode_Rising);
}

GpsTransactionRecorder::~GpsTransactionRecorder()
{
    delete _reader;
    delete _writer;
}

void GpsTransactionRecorder::do_work()
{
    /// @todo We should have a timer for reading the serial port so it never gets empty.
    read_serial_port();

    // Write out any transactions
    _writer->write_transactions();
}

void GpsTransactionRecorder::read_serial_port()
{
    // Process anything on the serial port
    /// @todo We want to limit the time spent here. Move this to a timer.
    while(_gps_serial_interface->available())
    {
        char c = _gps_serial_interface->read();
        if (_reader->parse_char(c))
        {
            GpsTransaction *transaction = new GpsTransaction(mcu_microseconds(), GpsTransaction::NMEA_sentence, _nmea_parser_output_buffer);
            if (transaction)
            {
                _writer->push(transaction);
            }
        }
    }
}

void GpsTransactionRecorder::on_high_interrupt()
{
    GpsTransaction *transaction = new GpsTransaction(mcu_microseconds(), GpsTransaction::PPS_pulse);
    if (transaction)
    {
        _writer->push(transaction);
    }
}
