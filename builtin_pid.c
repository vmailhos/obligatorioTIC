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

//nunca puede retornar null
int builtin_pid(int argc, char **argv) {
    printf("Process ID: %d\n", getpid());
    return 0;
}