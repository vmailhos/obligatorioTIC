#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "minish.h"
#include "wrappers.h"

//los printf se supone que son errores?
int externo (int argc, char ** argv){

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error en el fork.\n");

    } else if (pid == 0) {
        execvp(argv[0], argv);
        perror("Error en el execvp");

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
            char errorMessage[100];
            sprintf("El proceso hijo terminó debido a la señal %d\n", signal_number);
            perror(errorMessage);
       
        //Proceso hijo se detuvo por una senal
        } else if (WIFSTOPPED(status)) {
            int signal_number = WSTOPSIG(status);
            char errorMessage[100];
            sprintf("El proceso hijo se detuvo debido a la señal %d\n", signal_number);
            perror(errorMessage);
        }

    }

    return 1; 
}

