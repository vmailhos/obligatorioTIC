#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <errno.h>

#include "minish.h"
#include "wrappers.h"


int builtin_pid(int argc, char **argv) {
    pid_t pid = getpid();
    //nunca puede retornar null? preguntar
    printf("Process ID: %d\n", getpid());
    return 0;
}