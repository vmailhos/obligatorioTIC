#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "minish.h"
#include "wrappers.h"


//help 

int builtin_help (int argc, char ** argv){
    if (argv[1]== NULL){
        struct builtin_struct* builtin_help = builtin_lookup(argv[0]);;
        printf("%s\n", builtin_help->help_txt);
    }else {
        struct builtin_struct* builtin = builtin_lookup(argv[1]);
        if (builtin != NULL) {
            printf("%s\n", builtin->help_txt);
        } else {
            perror("No se encontro el comando:"); //nos tira undefined error: 0
            return 1;        
            }  
    }
    return 0;
}