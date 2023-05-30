#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int statusValue; 

int builtin_status (void){
    printf("status: %d\n", statusValue); //no se si tiene que imprimir esto en realidad. probar ocn la funcion en terminal
    return 0; 
}

//no puede tener errores no?