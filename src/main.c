#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/pclock.h"
#include "../include/parse.h"
#include "../include/util.h"
#include "../include/procutil.h"


#define CHILD_PROCESS 0
#define CLOCK_KEY 8675309
#define PROC_KEY 3141579


int main(int argc, char* argv[]) {
    parse_options(argc, argv);
    
    unsigned int max_child_process_count = get_max_child_process_count();


    // Initialize the system clock
    int clock_shid;
    if ((clock_shid = init_clock(CLOCK_KEY)) == -1) {
        perror("Failed to initialize system clock.");
        return EXIT_FAILURE;
    }

    // Spawn off `max_child_process_count` children.
    int i;
    pid_t spawned_proc_id;
    for (i = 0; i < max_child_process_count; ++i) {
        spawned_proc_id = fork();
        if (spawned_proc_id == CHILD_PROCESS) {
            break;
        }
    }

    // XXX: Debug, remove.
    if (spawned_proc_id == CHILD_PROCESS) {
        fprintf(stderr, "[+] PID: %ld; PPID: %ld\n", (long) getpid(), (long) getppid());
    }

    // Loop and check shared memory 
    // TODO: Move child pid termination checking code
    // and semaphors to a sep file. 

    // Wait for all child processes to terminate.
    if (spawned_proc_id) {
        while (get_nano() < 100) {
            tick(1);
            fprintf(stderr, "System Time %d\n", get_nano());
        }
        int status = 0;
        pid_t wait_pid;
        while((wait_pid = wait(&status)) > 0);
        if (destruct_clock(CLOCK_KEY, clock_shid) == -1) {
            perror("Failed to deallocate clock");
        }
    } else {
        execl("user", "user", NULL);
        //fprintf(stderr, "[%ld] System Time: %d\n", (long) getpid(), get_nano());
    }

    return EXIT_SUCCESS;    
}