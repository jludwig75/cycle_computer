#pragma once

#include "interrupt_modes.h"

#include <stdint.h>


#define MAX_NUMBER_OF_INTERRUPTS  6


class InterruptClass
{
  public:
    virtual void on_change_interrupt() {}
    virtual void on_high_interrupt() {}
    virtual void on_low_interrupt() {}
};

class InterruptManagerClass
{
  public:
    InterruptManagerClass();
    bool attachInterrupt(uint8_t interruptPin, InterruptClass *class_instance, void (InterruptClass::*instance)(), InterruptMode mode);
    void(*_isr_table[MAX_NUMBER_OF_INTERRUPTS])();
  private:
};

InterruptMode arduino_to_interrupt_mgr_mode(int mode);
int interrupt_mgr_to_arduino_mode(InterruptMode mode);

extern InterruptManagerClass InterruptManager;
