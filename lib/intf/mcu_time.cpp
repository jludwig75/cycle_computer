#include <Arduino.h>

#include "mcu_time.h"

uint32_t mcu_micros()
{
    return micros();
}

uint32_t mcu_millis()
{
    return millis();
}

uint64_t mcu_microseconds()
{
    return ((uint64_t)millis()) * 1000 + micros();
}