#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"

int statusValue = 0;

//Funcion para ejecutar comandos
int ejecutar (int argc, char ** argv){
    //Buscar el nombre del comando entre los nombres de los comandos internos
    struct builtin_struct* builtin = builtin_lookup(argv[0]);
    
    if (builtin != NULL) {
        //Si el comando es interno, ejecutar el comando interno y almacenar el resultado en statusValue
        int comando_interno = (*builtin->func)(argc, argv);
        statusValue=comando_interno;  
        return statusValue;

    } else {
        //Si el comando no es interno, ejecutar el comando externo y almacenar el resultado en el statusValue
        statusValue=externo(argc, argv);
        return statusValue;
    }
}