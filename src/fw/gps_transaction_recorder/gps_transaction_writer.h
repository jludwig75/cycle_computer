#pragma once


class TransactionLogger;
class GpsTransaction;


class GpsTransactionWriter
{
public:
    GpsTransactionWriter(TransactionLogger &transaction_logger);
    void push(GpsTransaction *transaction);

    void write_transactions();

private:
    GpsTransaction *pop();

    TransactionLogger &_transaction_logger;
    GpsTransaction *_queue_head;
    GpsTransaction *_queue_tail;
};