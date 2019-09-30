#include <stdio.h>
#include <unistd.h>

#include "../include/util.h"
#include "../include/pclock.h"

#define RUN_TIME_MIN 0
#define RUN_TIME_MAX 1000000

// XXX - This should really be based on path or something
#define KEY 8675309


int main(int argc, char* argv[]) {
    unsigned int run_time;
    run_time = rand_between(RUN_TIME_MIN, RUN_TIME_MAX);

    // Initialize the system clock
    int system_clock_shid = init_clock(KEY);

    fprintf(stderr, "[%ld] System Time: %d\n", (long) getpid(), get_nano());

    //pclock_t stop_time;
    //pclock_t local_clock;
    //get_copy(&local_clock);
    //stop_time = add(local_clock, run_time);
}