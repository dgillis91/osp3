#ifndef _PARSE_H
#define _PARSE_H


typedef struct program_options {
    char logfile_path[200];
    unsigned int max_child_process_count;
    unsigned int allowable_run_time;
} program_options_t;


void parse_options(int, char**);


void print_help_and_terminate(char*);


void set_default_program_options();


unsigned int get_allowable_run_time();


unsigned int get_max_child_process_count();


const char* get_logfile_path();

#endif
