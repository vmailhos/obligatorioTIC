#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "wrappers.h"

int builtin_help (int argc, char ** argv){

    if (argv[1]== NULL){
        printf("\n");
        printf("Comandos internos:\n");
        printf("- exit [n]: exit the shell.\n");
        printf("- pid: get process identification.\n");
        printf("- uid: get User ID as a number and the username.\n");
        printf("- history [N]: display the history list.\n");
        printf("- status: \n");
        printf("- cd [dir]: change the shell working directory.\n");
        printf("- dir [text/directory]:\n");
        printf("- getenv: \n");
        printf("- gid: \n");
        printf("- setenv: \n");
        printf("- unsetenv: \n");
        printf("- ejecutar: \n");
        printf("- externo: \n");
        printf("\n");
    }else {
        struct builtin_struct* builtin = builtin_lookup(argv[1]);
        if (builtin != NULL) {
            printf("%s\n", builtin->help_txt);
        } else {
            printf("Error");
        }  
    }

}