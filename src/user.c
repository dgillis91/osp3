#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/util.h"
#include "../include/pclock.h"
#include "../include/procutil.h"

#define RUN_TIME_MAX 1000000

// XXX - This should really be based on path or something
#define CLOCK_KEY 8675309
#define PROC_KEY 3141579


int main(int argc, char* argv[]) {
    srand(time(NULL) ^ (getpid() << 16));
    unsigned int run_time;
    run_time = rand_below(RUN_TIME_MAX);

    // Initialize the system clock
    int system_clock_shid = init_clock(CLOCK_KEY);
    int proc_shid = init_proc_handle(PROC_KEY);

    // Compute stop time.
    pclock_t stop_time;
    pclock_t local_clock;
    get_copy(&local_clock);
    stop_time = add(local_clock, run_time);

    // Get busy - check your clock.
    while (1) {
        if (get_total_tick() >= stop_time.total_tick || get_is_abrupt_terminate()) {
            mark_ready_to_terminate();
            break;
        }
    }

    return EXIT_SUCCESS;
}