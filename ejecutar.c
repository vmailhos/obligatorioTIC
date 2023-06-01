#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"


int statusValue = 0;

int ejecutar (int argc, char ** argv){
    struct builtin_struct* builtin = builtin_lookup(argv[0]);
    if (builtin != NULL) {
        int comando_interno = (*builtin->func)(argc, argv);
        statusValue=comando_interno;  
        return statusValue;
    } else {
        statusValue=externo(argc, argv);
        return statusValue;
    }
}