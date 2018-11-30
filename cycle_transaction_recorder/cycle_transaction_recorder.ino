#include "transaction_logger/transaction_logger.h"
#include "gps_transaction_recorder/gps_transaction_recorder.h"


SerialInterface &gps_serial; // = Get the GPS Serial port @todo

class CycleTransactionRecorderApp
{
public:
    CycleTransactionRecorderApp(SerialInterface &gps_serial, const char *transaction_log_file_name) :
        _transaction_logger(transaction_log_file_name),
        _gps_recorder(gps_serial, _transaction_logger)
    {

    }
    void begin()
    {

    }
    void on_loop()
    {
        _gps_recorder.do_work();
    }
private:
    TransactionLogger _transaction_logger;
    GpsTransactionRecorder _gps_recorder;
};


CycleTransactionRecorderApp app(gps_serial, "/cycle_transactions.log");

void setup()
{
    app.begin();
}

void loop()
{
    app.on_loop();
}