#pragma once

#include "ulator/ulator.h"


class SerialInterface;

class GpsUlator : public Ulator
{
public:
    GpsUlator(SerialInterface *device_serial_interface);

    // Ulator
    bool start();
    void stop();

private:
    SerialInterface *_device_serial_interface;
};