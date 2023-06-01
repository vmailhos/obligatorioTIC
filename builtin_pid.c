#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <sys/types.h>
#include <unistd.h>

#include "minish.h"
#include "wrappers.h"


int builtin_pid(int argc, char **argv) {
    
    pid_t pid = getpid();
    pid_t ppid = getppid();
  
    printf("pid: %d, ppid: %d\n", getpid(), getppid());

    return 0;
}