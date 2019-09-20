#include <stdio.h>
#include <stdlib.h>


#include "../include/parse.h"

int main(int argc, char* argv[]) {
    parse_options(argc, argv);

    printf("Allowable Run Time: %d\n", get_allowable_run_time());
    printf("Log Path: %s\n", get_logfile_path());
    printf("Max Children: %d\n", get_max_child_process_count());

    return EXIT_SUCCESS;    
}