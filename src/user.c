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

    pclock_t stop_time;

    stop_time = add
}