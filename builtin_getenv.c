#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "minish.h"
#include "wrappers.h"

// Función para imprimir el valor de variables de entorno
// Si hay un error, retorna 1; de lo contrario, retorna 0
int builtin_getenv (int argc, char ** argv){
    char* variable;
    char* valor;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        valor = getenv(variable); // Obtener el valor de la variable de entorno
        if(valor!=NULL){
            printf("%s = %s\n", variable, valor);// Imprimir la variable y su valor
        }else{
            fprintf(stderr, "getenv %s error\n", variable); 
            return 1; 
        }
    }
    return 0;
}