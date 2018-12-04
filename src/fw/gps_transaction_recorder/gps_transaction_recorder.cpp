#include "gps_transaction_recorder.h"

#include "mcu_time.h"
#include "serial_interface.h"
#include "nmea_sentence_reader/nmea_sentence_reader.h"
#include "transaction_logger/transaction_logger.h"
#include "gps_transaction.h"

GpsTransactionRecorder::GpsTransactionRecorder(TransactionLogger &transaction_logger) :
    _gps_serial_interface(NULL),
    _transaction_logger(transaction_logger),
    _nmea_parser_output_buffer(NULL),
    _nmea_parser_output_buffer_size(20),    /// @todo get a max size from the reader.
    _reader(NULL)
{
    _nmea_parser_output_buffer = new char[_nmea_parser_output_buffer_size + 1];
    _reader = new NmeaSentenceReader(_nmea_parser_output_buffer, _nmea_parser_output_buffer_size);
}

void GpsTransactionRecorder::begin(SerialInterface *gps_serial_interface)
{
    _gps_serial_interface = gps_serial_interface;
}

GpsTransactionRecorder::~GpsTransactionRecorder()
{
    delete _reader;
}

void GpsTransactionRecorder::do_work()
{
    // Process anything on the serial port
    /// @todo We may want to limit the time spent here.
    while(_gps_serial_interface->available())
    {
        char c = _gps_serial_interface->read();
        if (_reader->parse_char(c))
        {
            GpsTransaction transaction(mcu_microseconds(), GpsTransaction::NMEA_sentence, _nmea_parser_output_buffer);
            _transaction_logger.write_transaction(transaction);
        }
    }
}
