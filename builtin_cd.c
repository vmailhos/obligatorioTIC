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
    char *copy_of_PWD; 

    if (argc>2){
        perror("Error, numero incorrecto de argumentos\n"); //me tira undefined error 0 
        return 1;
    }

    //cuando solo está el comando
    else if (argc==1 && home_dir != NULL){
        setenv("OLDPWD", getenv("PWD"), 1);
        if(chdir(home_dir)==-1){
            perror("No se pudo cambiar a directorio HOME \n");
            return 1;
        }
        if (setenv("PWD", home_dir, 1)==-1){
            perror("No se pudo cambiar a directorio HOME\n");
            return 1;
        }
    }

    //cuando hay dos argumentos. 
    else if (strcmp(argv[1],"-")==0 && getenv("OLDPWD") != NULL){    

        //errores
        if((chdir(getenv("OLDPWD")))==-1){
            perror("No se pudo cambiar a directorio anterior\n");
            return 1;
        }
        copy_of_PWD=strdup_or_exit(getenv("PWD"));
        if (setenv("PWD", getenv("OLDPWD"), 1)==-1){
            perror("No se pudo cambiar a directorio anterior\n");
            return 1;
        }
        if (setenv("OLDPWD", copy_of_PWD, 1)==-1){
            perror("No se pudo cambiar a directorio anterior\n");
            return 1;
        }
        free(copy_of_PWD);
    }
    
    else { 
        // Cambiar al directorio especificado
        next_dir= argv[1];
        if (chdir(next_dir)==-1) {
            perror("No se pudo cambiar a directorio indicado\n");
            return 1;
        }
        char absolute_path[1024]; //usar getcwd
        char *result = realpath(next_dir, absolute_path);
            
        if (result != NULL) {
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", absolute_path, 1);
        }
        else {
            perror("Directorio no encontrado\n");
            return 1;
        }
    }

    return 0;
}

