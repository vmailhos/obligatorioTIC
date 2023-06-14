#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <sys/types.h>
#include <unistd.h>

#include "minish.h"
#include "wrappers.h"

int builtin_pid(int argc, char **argv) { 

    (void)argc;
    (void)argv;

    pid_t id = getpid(); // variable para almacenar el function's process id
    pid_t ppid = getppid(); // variable para almacenar el parent function's process id

    printf("pid: %d, ppid: %d\n", id, ppid);
    return 0;
    // pid y ppid siempre son exitosas (no hay que revisar errores)

}