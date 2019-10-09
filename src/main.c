#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/pclock.h"
#include "../include/parse.h"
#include "../include/util.h"
#include "../include/procutil.h"


#define TOTAL_ALLOWABLE_PROCESSES 100
#define TICK_INCREMENT 5
#define CHILD_PROCESS 0
#define CLOCK_KEY 8675309
#define PROC_KEY 3141579

int log_file_fd, clock_shid, proc_shid;


void terminate_program();


int main(int argc, char* argv[]) {
    parse_options(argc, argv);
    
    unsigned int max_child_process_count = get_max_child_process_count();
    log_file_fd = open(get_logfile_path(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // Initialize the system clock
    if ((clock_shid = init_clock(CLOCK_KEY)) == -1) {
        perror("Failed to initialize system clock.");
        return EXIT_FAILURE;
    }
    if ((proc_shid = init_proc_handle(PROC_KEY)) == -1) {
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

    // Loop and check shared memory 
    unsigned int max_run_time = get_allowable_run_time();
    unsigned int process_count = get_max_child_process_count();
    max_run_time *= NANO_SEC_IN_SEC;
    if (spawned_proc_id) {
        int stat;
        pid_t wait_pid;
        // While we are less than the run time
        while (get_total_tick() < max_run_time && process_count < TOTAL_ALLOWABLE_PROCESSES) {
            // Tick the clock
            tick(TICK_INCREMENT);
            // If we have procs ready to terminate . . . 
            if (get_count_procs_ready_terminate() > 0) {
                // Signify that we are adding another, decreasing the count
                mark_terminate();
                // XXX: This wait is actually not really effective. We require
                //      two way communication.
                wait_pid = wait(&stat);
                // Spawn a proc
                spawned_proc_id = fork();
                // If the process ID is the child, break out of the loop to exec.
                if (spawned_proc_id == CHILD_PROCESS) {
                    break;
                } else {
                    ++process_count;
                    fprintf(stderr, "Processes: %d\n", process_count);
                }
            }
        }
    } 

    if (spawned_proc_id) {
        if (process_count >= 100) {
            fprintf(stderr, "[!] Time in master %u\n", get_total_tick());
            set_is_abrupt_terminate();
        }
    }

    // Exec in the child
    if (spawned_proc_id == CHILD_PROCESS) {
        execl("user", "user", NULL);
    }

    // Wait for all child processes to terminate and clean up.
    if (spawned_proc_id) {
        terminate_program();
    } 

    return EXIT_SUCCESS;    
}

void terminate_program() {
    set_is_abrupt_terminate();
    int status = 0;
    pid_t wait_pid;
    while((wait_pid = wait(&status)) > 0) { 
        tick(TICK_INCREMENT);
        if (get_count_procs_ready_terminate() > 0) {
            mark_terminate();
        }
    }
    if (destruct_clock(CLOCK_KEY, clock_shid) == -1) {
        perror("Failed to deallocate clock");
    }
    if (destruct_proc_handle(PROC_KEY, proc_shid) == -1) {
        perror("Failed to deallocate proc_handle");
    }
    exit(EXIT_SUCCESS);
}