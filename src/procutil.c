#include "../include/procutil.h"
#include "../include/shmutil.h"
#include "../include/semutil.h"

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PROC_HANDLE_PERM (S_IRUSR | S_IWUSR)

#define FULL_BUFFER -3
#define EMPTY_BUFFER -2
#define NULL_PID (pid_t)(0)

static int semid;
static struct sembuf semlock;
static struct sembuf semunlock;
static proc_handle_t* proc_handle;


int init_proc_handle(int key) {
    int shid;

    setsembuf(&semlock, 0, -1, 0);
    setsembuf(&semunlock, 0, 1, 0);

    shid = shmget(key, sizeof(proc_handle_t), PROC_HANDLE_PERM | IPC_CREAT | IPC_EXCL);

    if ((shid == -1) && errno != EEXIST) {
        return -1;
    }

    if (shid == -1) {
        if ((shid = shmget(key, sizeof(proc_handle_t), PROC_HANDLE_PERM)) == -1) {
            return -1;
        }
        if ((proc_handle = (proc_handle_t*) shmat(shid, NULL, 0)) == (void*)(-1)) {
            return -1;
        }
    } else {
        proc_handle = (proc_handle_t*) shmat(shid, NULL, 0);
        if (proc_handle == (void*)(-1)) {
            return -1;
        }
    }
    semid = initsemset(key, 1, &proc_handle->ready);
    if (semid == -1) {
        return -1;
    }
    return shid;
}


void set_max_process_count(unsigned int max_process_count) {
    /* Not thread safe. Call AFTER init. */
    proc_handle->max_process_count = max_process_count;
}


unsigned int get_max_process_count() {
    return proc_handle->max_process_count;
}


int index_of(pid_t pid) {
    /* Return the index of `pid`.
    ** Uses linear search. Shouldn't be a huge problem. 
    ** Return -1 if index is not found.
    */
    int is_found = 0, i = 0;

    if (semop(semid, &semlock, 1) == -1) 
        return -1;

    for (i = 0; i < proc_handle->max_process_count; ++i) {
        if ((int) pid == (int) proc_handle->proc_handle_buffer[i]) {
            is_found = 1;
            break;
        }
    }

    if (semop(semid, &semunlock, 1) == -1)
        return -1;

    if (is_found) {
        return i;
    } else {
        return -1;
    }
}


int set_at(int index, pid_t pid) {
    if (semop(semid, &semlock, 1) == -1) 
        return -1;
    proc_handle->proc_handle_buffer[index] = pid;
    if (semop(semid, &semunlock, 1) == -1)
        return -1;
    return 1;
}


pid_t get_at(int index) {
    pid_t return_pid;
    if (semop(semid, &semlock, 1) == -1)
        return (pid_t)(0);
    return_pid = proc_handle->proc_handle_buffer[index];
    if (semop(semid, &semunlock, 1) == -1)
        return (pid_t)(0);
    return return_pid;
}


int clear() {
    int i = 0;
    if (semop(semid, &semlock, 1) == -1)
        return -1;
    for (i = 0; i < proc_handle->max_process_count; ++i) {
        proc_handle->proc_handle_buffer[i] = NULL_PID;
    }
    if (semop(semid, &semunlock, 1) == -1)
        return -1;
    return 1;
}


int index_of_next_unempty() {
    /* Get the index of the next pid in the buffer.
    ** Return:
    **  -1 on Failure
    **  EMPTY_BUFFER = -2 on empty
    **  index of the first non null pid, on success.
    */
    int i, is_found = 0;
    if (semop(semid, &semlock, 1) == -1) 
        return -1;
    for (i = 0; i < proc_handle->max_process_count; ++i) {
        if (proc_handle->proc_handle_buffer[i] != NULL_PID) {
            is_found = 1;
            break;
        }
    }
    if (semop(semid, &semunlock, 1) == -1)
        return -1;

    if (is_found) {
        return i;
    } else {
        return EMPTY_BUFFER;
    }
}


int place_at_first_empty(pid_t pid) {
    // XXX: There's a faster algorithm for this. 
    //      Just keep track of the next empty. IDK why
    //      I didn't think of this. 
    // TODO: Refactor if time allows. 
    int i, empty_exists = 0;
    if (semop(semid, &semlock, 1) == -1)
        return -1;
    for (i = 0; i < proc_handle->max_process_count; ++i) {
        if (proc_handle->proc_handle_buffer[i] == NULL_PID) {
            proc_handle->proc_handle_buffer[i] = pid;
            empty_exists = 1;
            break;
        }
    }
    if (semop(semid, &semlock, 1) == -1) 
        return -1;
    if (empty_exists) {
        return i;
    } else {
        return FULL_BUFFER;
    }
}
