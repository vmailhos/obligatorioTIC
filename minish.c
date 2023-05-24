#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wrappers.h"
#include "minish.h"


int main(int argc, char *argv[]){
    char *linea = "aaaa bbbb ccc       \n";
    linea2argv(linea, argc, argv);
    /*while(1){
        fprintf(stderr, "Mensaje de prompt en standard error: \n");   //para borrar
    }*/
}