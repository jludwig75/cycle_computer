#include "gps_transaction_writer.h"

#include "transaction_logger/transaction_logger.h"
#include "gps_transaction.h"

#include <stdlib.h>
#include <assert.h>


GpsTransactionWriter::GpsTransactionWriter(TransactionLogger &transaction_logger) :
    _transaction_logger(transaction_logger),
    _queue_head(NULL),
    _queue_tail(NULL)
{
}

void GpsTransactionWriter::push(GpsTransaction *transaction)
{
    /// @todo Synchronize for concurrent ISR/main thread use
    assert(transaction->next == NULL);

    if (_queue_head == NULL)
    {
        assert(_queue_tail == NULL);

        _queue_head = _queue_tail = transaction;
    }
    else
    {
        assert(_queue_tail != NULL);
        _queue_tail->next = transaction;
        _queue_tail = transaction;
    }
}

GpsTransaction *GpsTransactionWriter::pop()
{
    /// @todo Synchronize for concurrent ISR/main thread use
    if (_queue_head == NULL)
    {
        assert(_queue_tail == NULL);
        return NULL;
    }
    else
    {
        assert(_queue_tail != NULL);

        GpsTransaction *transaction = _queue_head;
        if (_queue_head == _queue_tail)
        {
            // There is only one entry.
            assert(_queue_head->next == NULL);
            _queue_head = _queue_tail = NULL;
        }
        else
        {
            // There must be another entry.
            assert(_queue_head->next != NULL);
            _queue_head = _queue_head->next;
        }

        transaction->next = NULL;
        return transaction;
    }
}

void GpsTransactionWriter::write_transactions()
{
    /// @todo We may want to limit the time spent here.
    GpsTransaction *transaction;
    while((transaction = pop()) != NULL)
    {
        _transaction_logger.write_transaction(*transaction);
        delete transaction;
    }
}
