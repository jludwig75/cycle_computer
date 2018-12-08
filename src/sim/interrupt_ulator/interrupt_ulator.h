#pragma once

#include "interrupt_modes.h"

#include "ulator/ulator.h"
#include <stdint.h>


class InterruptUlatorClass : public Ulator
{
public:
    // Ulator
    virtual bool start();
    virtual void stop();
    void trigger_interrupt(uint8_t pin_number, InterruptMode interrupt_type);
    void attach_interrupt(uint8_t pin_number, void (*isr)(void), InterruptMode mode);
};

extern InterruptUlatorClass InterruptUlator;