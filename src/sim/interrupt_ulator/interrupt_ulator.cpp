#include "interrupt_ulator.h"

#include "interrupt_manager/InterruptManager.h"

#include <map>

typedef std::map<InterruptMode, void (*)(void)> ModeInterrupts;
typedef std::map<uint8_t, ModeInterrupts> PinInterrupts;


static PinInterrupts interrupt_map;


InterruptUlatorClass InterruptUlator;


bool InterruptUlatorClass::start()
{
    return true;
}

void InterruptUlatorClass::stop()
{

}

void InterruptUlatorClass::trigger_interrupt(uint8_t pin_number, InterruptMode interrupt_type)
{
    PinInterrupts::iterator i = interrupt_map.find(pin_number);
    if (i == interrupt_map.end())
    {
        // No interrupts registered for this pin
        return;
    }

    ModeInterrupts::iterator m = interrupt_map[pin_number].find(interrupt_type);
    if (m == interrupt_map[pin_number].end())
    {
        // No interrupts registered onthis pin for this mode.
        return;
    }

    // Call the ISR
    interrupt_map[pin_number][interrupt_type]();
}

void InterruptUlatorClass::attach_interrupt(uint8_t pin_number, void (*isr)(void), InterruptMode mode)
{
    PinInterrupts::iterator i = interrupt_map.find(pin_number);
    if (i == interrupt_map.end())
    {
        // Make sure this a modes ISR list for this pin
        interrupt_map[pin_number] = ModeInterrupts();
    }

    // Set the ISR
    interrupt_map[pin_number][mode] = isr;
}


void attachInterrupt(uint8_t pin, void (*isr)(void), int mode)
{
    InterruptUlator.attach_interrupt(pin, isr, arduino_to_interrupt_mgr_mode(mode));
}