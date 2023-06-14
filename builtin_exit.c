#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"

// Se usa la funcion atoi(), si el string en argv[1] tiene un numero, lo castea a int y devuelve el status con ese int
// pero si se le pasa un string sin numeros, retorna status 0.

int builtin_exit (int argc, char ** argv){
    int status = 0;
    //Si la funcion tiene argumento numerico, hace exit con status de retorno igual al argumento
    if (argc > 1 ) {
        status = atoi(argv[1]);
    }
    //Sin argumento, exit con status de retorno igual a 0
    exit(status);
}