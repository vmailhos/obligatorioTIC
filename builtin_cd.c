#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "minish.h"
#include "wrappers.h"

//Funciones que manejan errores con perror y fprintf
int error_file(char *directory){
    char errorMessage[100];
    sprintf(errorMessage, "cd error: chdir %s:\n",directory);
    perror(errorMessage);       
    return 1;
}
int error_file1(char *directory){
    fprintf(stderr,"cd error: chdir %s:\n",directory);
    return 1;
}


//Funcion que cambia de directorio, recibe dos argumentos. Un array de punteros a char y un int indicando su tamano.
int builtin_cd (int argc, char ** argv){
    char *home_dir = getenv("HOME");
    char *next_dir;
    char *copy_of_PWD; 

    if (argc>2){
        fprintf(stderr,"cd: argumentos incorrectos - help cd para más ayuda\n"); 
        return 1;
    }

    //cuando solo está el comando
    else if (argc==1 && home_dir != NULL){
        setenv("OLDPWD", getenv("PWD"), 1);
        if(chdir(home_dir)==-1){
            fprintf(stderr,"No se pudo cambiar a directorio HOME \n");
            return 1;
        }
        if (setenv("PWD", home_dir, 1)==-1){
            fprintf(stderr,"No se pudo cambiar a directorio HOME\n");
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
            fprintf(stderr,"No se pudo cambiar a directorio anterior\n");
            return 1;
        }
        if (setenv("OLDPWD", copy_of_PWD, 1)==-1){
            fprintf(stderr,"No se pudo cambiar a directorio anterior\n");
            return 1;
        }
        free(copy_of_PWD); // Liberar la memoria asignada para la copia de PWD
    }
    
    else { 
        // Cambiar al directorio especificado
        next_dir= argv[1];
        if (chdir(next_dir)==-1) {
            return error_file(next_dir);
        }
        char absolute_path[1024];
        char *result = realpath(next_dir, absolute_path);
            
        if (result != NULL) {
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", absolute_path, 1);
        }
        else {
            return error_file1(next_dir);
        }
    }

    return 0;
}

