#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/util.h"
#include "../include/pclock.h"

#define RUN_TIME_MAX 1000000

// XXX - This should really be based on path or something
#define KEY 8675309


int main(int argc, char* argv[]) {
    srand(time(NULL) ^ (getpid() << 16));
    unsigned int run_time;
    run_time = rand_below(RUN_TIME_MAX);

    // Initialize the system clock
    int system_clock_shid = init_clock(KEY);

    pclock_t stop_time;
    pclock_t local_clock;
    get_copy(&local_clock);
    fprintf(stderr, "[%ld] Local Clock Time: %lu\n", (long) getpid(), local_clock.total_tick);
    stop_time = add(local_clock, run_time);
    fprintf(stderr, "[%ld] Local Clock Stop Time: %lu\n", (long) getpid(), stop_time.total_tick);

    return EXIT_SUCCESS;
}