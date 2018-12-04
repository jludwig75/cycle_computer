#include "mcu_time.h"

#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>


static uint64_t cpu_start_time;

uint32_t micros()
{
    timeval tv;
    int ret = gettimeofday(&tv, NULL);
    assert(ret == 0);

    uint64_t current_time = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
    assert(current_time >= cpu_start_time);

    return (uint32_t)(current_time - cpu_start_time);
}