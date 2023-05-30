#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


#include "minish.h"
#include "wrappers.h"

#define IN 1
#define OUT 0
#define MAXLINE 1024

int ejecutar (int argc, char ** argv){
    struct builtin_struct* builtin = builtin_lookup(argv[0]);
    if (builtin != NULL) {
        int comando_interno = (*builtin->func)(argc, argv);
        return comando_interno;  
    } else {
        return externo(argc, argv);  
    }
}

int externo (int argc, char ** argv){

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error en el fork.\n");
        return -1;

    } else if (pid == 0) {
        execvp(argv[0], argv);
        perror("Error en el execvp");
        return -1;

    } else {
        int status;
        waitpid(pid, &status, 0); //espera a que termine el hijo para tener el status
       
        //OPCIONES:
        //Proceso hijo termino correctamente
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            return exit_status;
               
        //Proceso hijo termino por una senal
        }else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            printf("El proceso hijo terminó debido a la señal %d\n", signal_number);
       
        //Proceso hijo se detuvo por una senal
        } else if (WIFSTOPPED(status)) {
            int signal_number = WSTOPSIG(status);
            printf("El proceso hijo se detuvo debido a la señal %d\n", signal_number);
        }

    }

    return -1;
}


int linea2argv(char *linea, int argc, char **argv) {  
    int i = 0;
    int c;
    int state = OUT;
    int wordcount = 0;
    int j = 0;
    char temp[MAXLINE];

    while ((c = linea[i]) != '\n') {
        //printf("%d -> ", i);
        //printf("%c\n", c);
        if (c == ' ' || c == '\t') {
            if (state == IN) {
                temp[j] = '\0';
                argv[wordcount] = strdup_or_exit(temp);
                wordcount++;
                j = 0;
                state = OUT;
            }
        } else {
            if (state == OUT) {
                state = IN;
            }
            temp[j] = linea[i];
            j++;
        }
        i++;
    }

    if (state == IN) {
        temp[j] = '\0';
        argv[wordcount] = strdup_or_exit(temp);
        wordcount++;
    }

    if (wordcount < argc) {
        argv[wordcount] = NULL;
    }

    return wordcount;
}