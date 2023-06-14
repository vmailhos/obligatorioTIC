#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "minish.h"
#include "wrappers.h"

int builtin_unsetenv (int argc, char ** argv){
    char* variable;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        if (unsetenv(variable) != 0) {
            fprintf(stderr,"Error al borrar: %s\n", variable);
            return 1;
        }else{
            printf("%s unset\n", argv[1]);
        }
    }
    return 0;
}
