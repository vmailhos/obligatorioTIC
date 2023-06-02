#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "minish.h"
#include "wrappers.h"

extern char *prev_dir; // ponerlo e main y actualizar ahi

//arreglar sprintf y el main como lo cambio gerardo a mery
//controlar errores de si te da mas de un argv en todas las funciones
//no funciona del todo bien a veces cambias de directorio y el anterior se mantiene igual
int builtin_cd (int argc, char ** argv){
    char *home_dir = getenv("HOME");
    char *next_dir;

    if (argc>2){
        perror("Error, numero incorrecto de argumentos"); //me tira undefined error 0 
        return 1;
    }

    //cuando solo está el comando
    else if (argc==1){
        if (home_dir != NULL) {
            chdir(home_dir);
            setenv("PWD", home_dir, 1);
        }
    }

    //cuando hay dos argumentos
    else { 
        if (argv[1][0]=='-'){
            if (prev_dir != NULL) {
                chdir(prev_dir);
                setenv("PWD", prev_dir, 1);
            }
        }
        else { 
            // Cambiar al directorio especificado
            next_dir= argv[1];
            if (chdir(next_dir) == 0) {
            char absolute_path[1024]; //usar getcwd
            char *result = realpath(next_dir, absolute_path);
                if (result != NULL) {
                    setenv("PWD", absolute_path, 1);
                }
            }
            else {
                perror("Directorio no encontrado\n");
                return 1;
            }
        }
    }    
    return 0;
}