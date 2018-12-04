#pragma once

#include <list>

#include "ulator/ulator.h"

#include "gps_transaction_recorder/gps_transaction.h"


class SerialInterface;

class GpsUlator : public Ulator
{
public:
    GpsUlator(SerialInterface *device_serial_interface);

    // Ulator
    bool start();
    void stop();

protected:
    void run();
    static void *gps_thread(void *instance);

private:
    SerialInterface *_device_serial_interface;
    unsigned long _thread_id;
    bool _exit_thread;
    std::list<GpsTransaction> _transactions;
};