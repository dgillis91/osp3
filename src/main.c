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
    int clock_shid, proc_shid;
    if ((clock_shid = init_clock(CLOCK_KEY)) == -1) {
        perror("Failed to initialize system clock.");
        return EXIT_FAILURE;
    }
    if ((proc_shid = init_proc_handle(PROC_KEY) == -1)) {
        perror("Failed to initialize process handle");
        destruct_clock(CLOCK_KEY, clock_shid);
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
    unsigned int max_run_time = get_allowable_run_time();
    if (spawned_proc_id) {
        // While we are less than the run time
        while (get_seconds() < max_run_time) {
            // Tick the clock
            tick(5);
            // If we have procs ready to terminate . . . 
            if (get_count_procs_ready_terminate() > 0) {
                // Signify that we are adding another, decreasing the count
                mark_terminate();
                // Spawn a proc
                spawned_proc_id = fork();
                // If the process ID is the child, break out of the loop to exec.
                if (spawned_proc_id == CHILD_PROCESS) {
                    break;
                }
            }
        }
    } 

    // Exec in the child
    if (spawned_proc_id == CHILD_PROCESS) {
        execl("user", "user", NULL);
    }

    // Wait for all child processes to terminate and clean up.
    if (spawned_proc_id) {
        int status = 0;
        pid_t wait_pid;
        while((wait_pid = wait(&status)) > 0);
        if (destruct_clock(CLOCK_KEY, clock_shid) == -1) {
            perror("Failed to deallocate clock");
        }
        if (destruct_proc_handle(PROC_KEY, proc_shid) == -1) {
            perror("Failed to deallocate proc_handle");
        }
    } 

    return EXIT_SUCCESS;    
}