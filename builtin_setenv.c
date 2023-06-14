#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "minish.h"
#include "wrappers.h"

// Función para definir variables de entorno
// Si hay un error, retorna 1; de lo contrario, retorna 0
int builtin_setenv (int argc, char ** argv){
    if(argc!=3){
        fprintf(stderr,"setenv debe tener solo dos argumentos: variable y valor\n");
        return 1;
    }
    char* variable = argv[1];
    char* valor = argv[2];
    
    // Definir la variable de entorno utilizando setenv
    // El tercer argumento 1 indica que se sobrescribirá la variable si ya existe
    if (setenv(variable, valor, 1) != 0) {
        fprintf(stderr,"Error al definir: %s\n", variable);
        return 1;
    }
    return 0;
}