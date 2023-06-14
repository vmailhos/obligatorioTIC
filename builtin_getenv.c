#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "minish.h"
#include "wrappers.h"

//si hay error retorna 1, sino retorna 0
int builtin_getenv (int argc, char ** argv){
    char* variable;
    char* valor;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        valor = getenv(variable);
        if(valor!=NULL){
            printf("%s = %s\n", variable, valor);
        }else{
            //la funcion perror solo puede imprimir un mensaje --> creamos el mensaje de antemano
            fprintf(stderr, "getenv %s error\n", variable); // nos tira undefined error
            return 1; 
        }
    }
    return 0;
}