#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"

int builtin_exit (int argc, char ** argv){
    int last_exit_status = 0;
    int status = last_exit_status;

    if (argc > 1) {
        status = atoi(argv[1]);
    }

    exit(status);
}