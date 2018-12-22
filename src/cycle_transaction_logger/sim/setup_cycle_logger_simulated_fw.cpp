#include "transaction_loader/transaction_loader.h"

#include <stdlib.h>


TransactionLoader transaction_loader;


void setup_app_fw_sim()
{
    if (!transaction_loader.load_transactions(""))
    {
        fprintf(stderr, "Failed to load transactions from transaction log.\n");
    }
}