#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "../include/semutil.h"
#include "../include/pclock.h"
#include "../include/parse.h"


#include "../include/parse.h"


#define CHILD_PROCESS 0


int main(int argc, char* argv[]) {
    parse_options(argc, argv);
    
    unsigned int max_child_process_count = get_max_child_process_count();

    pid_t* child_process_handle = NULL;
    if ((child_process_handle = (pid_t*) malloc(sizeof(pid_t) * max_child_process_count)) == NULL) {
        perror("Failure to allocate memory for pids");
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
        child_process_handle[i] = spawned_proc_id;
    }

    if (spawned_proc_id == CHILD_PROCESS) {
        fprintf(stderr, "[+] PID: %ld; PPID: %ld\n", (long) getpid(), (long) getppid());
    }

    // Loop and check shared memory 
    // TODO: Move child pid termination checking code
    // and semaphors to a sep file. 
    while (1) {
        
    }

    // Wait for all child processes to terminate.
    if (spawned_proc_id) {
        int status = 0;
        pid_t wait_pid;
        while((wait_pid = wait(&status)) > 0) ;
    }

    return EXIT_SUCCESS;    
}