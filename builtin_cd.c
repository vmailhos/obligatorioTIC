#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "minish.h"
#include "wrappers.h"


//arreglar sprintf y el main como lo cambio gerardo a mery
//controlar errores de si te da mas de un argv en todas las funciones
//no funciona del todo bien a veces cambias de directorio y el anterior se mantiene igual
int builtin_cd (int argc, char ** argv){
    char *home_dir = getenv("HOME");
    char *next_dir;
    static char *prev_dir;
    static char *actual_dir;
    actual_dir= getenv("PWD");

    if (argc>2){
        perror("Error, numero incorrecto de argumentos"); //me tira undefined error 0 
        return 1;
    }

    //cuando solo está el comando
    else if (argc==1 && home_dir != NULL){
        prev_dir=actual_dir;
        chdir(home_dir);
        setenv("PWD", home_dir, 1);
        actual_dir=home_dir;
        if (prev_dir!=NULL){
            printf("prev: %s \n", prev_dir); 
            printf("actual:%s \n", actual_dir); 
        }
    }

    //cuando hay dos argumentos. 
    else if (argv[1][0]=='-' && prev_dir != NULL){    
        chdir(prev_dir);
        setenv("PWD", prev_dir, 1);
        prev_dir=actual_dir;
        actual_dir=getenv("PWD");
        if (prev_dir!=NULL){
            printf("prev: %s \n", prev_dir); 
            printf("actual:%s \n", actual_dir); 
        }
    }
    
    else { 
        // Cambiar al directorio especificado
        next_dir= argv[1];
        if (chdir(next_dir) == 0) {
            char absolute_path[1024]; //usar getcwd
            char *result = realpath(next_dir, absolute_path);
            if (result != NULL) {
                prev_dir=actual_dir;
                setenv("PWD", absolute_path, 1);
                actual_dir=getenv("PWD");
            }
        }
        else {
            perror("Directorio no encontrado\n");
            return 1;
        }
        if (prev_dir!=NULL){                
            printf("prev: %s \n", prev_dir); 
            printf("actual:%s \n", actual_dir); 
        }    
    return 0;
    }
}