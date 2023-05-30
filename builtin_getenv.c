#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"


int builtin_getenv (int argc, char ** argv){
    char* variable;
    char* valor;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        valor = getenv(variable);
        if(valor!=NULL){
            printf("%s = %s\n", variable, valor);
        }else{
            printf("%s = (NO ENCONTRADO)\n", variable);
        }
    }
    return 0;
}