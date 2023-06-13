#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

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
        perror("execvp error\n");
        exit(77);
    } else {
        struct sigaction str_sigint_action_old;
        struct sigaction str_sigint_action_new;
        memset(&str_sigint_action_new, 0, sizeof(str_sigint_action_new));
        str_sigint_action_new.sa_handler = SIG_IGN;

        sigaction(SIGINT, &str_sigint_action_new, &str_sigint_action_old);

        int status;
        waitpid(pid, &status, 0); //espera a que termine el hijo para tener el status

        sigaction(SIGINT, &str_sigint_action_old, NULL);

        //OPCIONES:
        //Proceso hijo termino correctamente
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            return exit_status;
        //Proceso hijo termino por una senal
        }else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            fprintf(stderr, " : Hijo recibio señal %d\n", signal_number);
        //Proceso hijo se detuvo por una senal
        } else if (WIFSTOPPED(status)) {
            int signal_number = WTERMSIG(status);
            fprintf(stderr, " : Hijo recibio señal  %d\n", signal_number);
        }
    }

    return 1; 
}

