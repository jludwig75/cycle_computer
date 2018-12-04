#include "transaction_logger/transaction_logger.h"
#include "gps_transaction_recorder/gps_transaction_recorder.h"
#include "uart.h"


class CycleTransactionLoggerApp
{
public:
    CycleTransactionLoggerApp(const char *transaction_log_file_name) :
        _transaction_logger(transaction_log_file_name),
        _gps_recorder(_transaction_logger)
    {

    }
    void begin()
    {
        _gps_recorder.begin(uart__get_serial_interface(1));
    }
    void on_loop()
    {
        _gps_recorder.do_work();
    }
private:
    TransactionLogger _transaction_logger;
    GpsTransactionRecorder _gps_recorder;
};


CycleTransactionLoggerApp app("/cycle_transactions.log");

void setup()
{
    app.begin();
}

void loop()
{
    app.on_loop();
}