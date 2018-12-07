#include "transaction_logger/transaction_logger.h"
#include "gps_transaction_recorder/gps_transaction_recorder.h"
#include "uart.h"

#include "SPIFFS.h"


class CycleTransactionLoggerApp
{
public:
    CycleTransactionLoggerApp(const char *transaction_log_file_name) :
        _transaction_logger(transaction_log_file_name),
        _gps_recorder(_transaction_logger)
    {

    }
    bool begin()
    {
        if (!SPIFFS.begin(true))
        {
            return false;
        }
        _gps_recorder.begin(uart__get_serial_interface(1));
        return true;
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
bool app_started;

void setup()
{
    app_started = app.begin();
    if (!app_started)
    {
        printf("Failed to start app\n");
    }
}

void loop()
{
    if (app_started)
    {
        app.on_loop();
    }
}