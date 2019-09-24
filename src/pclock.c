#include "../include/pclock.h"

// TODO: Add shared memory.
static pclock_t system_clock = {0, 0, 0};


pclock_t get_copy() {
    pclock_t copy;
    copy.nanoseconds = system_clock.nanoseconds;
    copy.seconds = system_clock.seconds;
    return copy;
}


void tick(int nanoseconds) {
    /* Increment the system clock by `nanoseconds`
    */
    add_in_place(&system_clock, nanoseconds);
}


pclock_t add(pclock_t clock, unsigned int nanoseconds) {
    add_in_place(&clock, nanoseconds);
    return clock;
}


void add_in_place(pclock_t* clock, unsigned int nanoseconds) {
    clock->total_tick += nanoseconds;
    clock->nanoseconds = clock->total_tick % NANO_SEC_IN_SEC;
    clock->seconds = clock->total_tick / NANO_SEC_IN_SEC;
}


int is_equal_to_sys_clock(pclock_t clock) {
    return clock.total_tick == system_clock.total_tick;
}
