#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"

int builtin_setenv (int argc, char ** argv){
    if(argc!=3){
        fprintf(stderr, "Faltan o sobran argumentos.\n");
        return 1;
    }
    char* variable = argv[1];
    char* valor = argv[2];

    if (setenv(variable, valor, 1) != 0) {
        fprintf(stderr, "Error al definir: %s\n", variable);
        return 1;
    }

    return 0;
}