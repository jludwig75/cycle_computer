#pragma once


class SerialInterface;
class TransactionLogger;


class GpsTransactionRecorder
{
public:
    GpsTransactionRecorder(SerialInterface *gps_serial_interface, TransactionLogger &transaction_logger);
    void do_work();
};


