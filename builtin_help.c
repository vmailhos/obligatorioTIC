#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "minish.h"
#include "wrappers.h"


//help 

int builtin_help (int argc, char ** argv){
    if (argv[1]== NULL){
        printf("help [cd|dir|exit|help|getenv|pid|setenv|status|uid|unsetenv]\n");
    }else {
        struct builtin_struct* builtin = builtin_lookup(argv[1]);
        if (builtin != NULL) {
            printf("%s\n", builtin->help_txt);
        } else {
            perror("Error, no se pudo encontrar el grupo principal"); //nos tira undefined error: 0
            return 1;        
            }  
    }
    return 0;
}