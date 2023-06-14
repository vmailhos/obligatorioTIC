#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "minish.h"
#include "wrappers.h"


int builtin_help (int argc, char ** argv){
    (void)argc;

    //Caso en el que no hay argumento, imprime el texto de ayuda del builtin_help (todos los nombres de los comandos)
    if (argv[1]== NULL){
        struct builtin_struct* builtin_help = builtin_lookup(argv[0]);;
        printf("%s\n", builtin_help->help_txt);

    //Caso en que hay argumento, busca el nombre del comando e imprime el texto de ayuda dicho comando 
    }else {
        struct builtin_struct* builtin = builtin_lookup(argv[1]);
        if (builtin != NULL) {
            printf("%s\n", builtin->help_txt);

        //Caso donde no fue encontrado el nombre del comando, status de retorno 1
        } else {
            fprintf(stderr,"No se encontro el comando %s \n",argv[1]);      
            return 1;
             //nos tira undefined error: 0     
            }  
    }
    return 0;
}