#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "minish.h"
#include "wrappers.h"

int builtin_setenv (int argc, char ** argv){
    if(argc!=3){
        perror("setenv debe tener solo dos argumentos: variable y valor\n");
        return 1;
    }
    char* variable = argv[1];
    char* valor = argv[2];

    if (setenv(variable, valor, 1) != 0) {
        char errorMessage[100];
        sprintf(errorMessage, "Error al definir: %s\n", variable);
        perror(errorMessage);
        return 1;
    }
    return 0;
}