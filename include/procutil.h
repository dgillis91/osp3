#ifndef PROC_UTIL_H
#define PROC_UTIL_H

#include <unistd.h>
#include <signal.h>

#define MAX_PROC_COUNT 100


typedef struct proc_handle {
    unsigned int max_process_count;
    pid_t proc_handle_buffer[MAX_PROC_COUNT];
    sig_atomic_t ready;
} proc_handle_t;


int init_proc_handle(int key);
int destruct_proc_handle(int key, int shid);
void set_max_process_count(unsigned int max_process_count);
unsigned int get_max_process_count();
int index_of(pid_t pid);
int set_at(int index, pid_t pid);

#endif
