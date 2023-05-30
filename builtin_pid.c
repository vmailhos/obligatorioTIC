#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"


int builtin_pid(int argc, char **argv) {
    
    printf("Process ID: %d\n", getpid());
    return 0;
}