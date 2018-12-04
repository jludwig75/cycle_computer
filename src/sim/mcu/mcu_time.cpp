#include "mcu_time.h"

#include "mcu_time_sim.h"

#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>


static uint64_t cpu_start_time_us;


static uint64_t get_microseconds_since_epoch()
{
    timeval tv;
    int ret = gettimeofday(&tv, NULL);
    assert(ret == 0);

    uint64_t current_time = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
    return current_time;
}

void start_cpu_clock()
{
    cpu_start_time_us = get_microseconds_since_epoch();
}

uint32_t mcu_micros()
{
    uint64_t current_time_us = get_microseconds_since_epoch();
    assert(current_time_us >= cpu_start_time_us);

    return (uint32_t)(current_time_us - cpu_start_time_us);
}

uint32_t mcu_millis()
{
    uint64_t current_time_us = get_microseconds_since_epoch();
    assert(current_time_us >= cpu_start_time_us);

    return (uint32_t)((current_time_us - cpu_start_time_us) / 1000);
}

uint64_t mcu_microseconds()
{
    return ((uint64_t)mcu_millis()) * 1000 + mcu_micros();
}