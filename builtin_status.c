#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Función para imprimir el valor de statusValue
extern int statusValue; 

int builtin_status (void){
    printf("%d\n", statusValue); 
    return 0; 
}

