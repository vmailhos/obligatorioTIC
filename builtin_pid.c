#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <sys/types.h>
#include <unistd.h>

#include "minish.h"
#include "wrappers.h"

//nunca puede retornar null
int builtin_pid(int argc, char **argv) {  
    printf("pid: %d, ppid: %d\n", getpid(), getppid());
    return 0;
}