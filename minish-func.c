#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <limits.h>

#include "minish.h"
#include "wrappers.h"
#define IN 1
#define OUT 0
#define MAXLINE 1024

int builtin_exit (int argc, char ** argv){
    int last_exit_status = 0;
    int status = last_exit_status;

    if (argc > 1) {
        status = atoi(argv[1]);
    }

    exit(status);
}

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

int builtin_history (int argc, char ** argv){
    return NULL;
}
int builtin_status (int argc, char ** argv){
    return NULL;
}
int builtin_cd (int argc, char ** argv){
    return NULL;
}
int builtin_dir (int argc, char ** argv){
    return NULL;
}
int builtin_getenv (int argc, char ** argv){
    char* variable;
    char* valor;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        valor = getenv(variable);
        if(valor!=NULL){
            printf("%s = %s\n", variable, valor);
        }else{
            printf("%s = (NO ENCONTRADO)\n", variable);
        }
    }
    return 0;
}

int builtin_gid (int argc, char ** argv){
    gid_t gid_principal = getegid();
    //printf("%d", gid_principal);
    
    struct group *grupo_principal = getgrgid(gid_principal);
    if(grupo_principal == NULL){
        return EXIT_FAILURE;
    }
    printf("Grupo Principal: %s\n", grupo_principal->gr_name);

    gid_t groups[NGROUPS_MAX];
    int num_grupos_segundarios = getgroups(NGROUPS_MAX, groups);
    //printf("%d\n", num_grupos_segundarios);
    if(num_grupos_segundarios == -1){
        return EXIT_FAILURE;
    }

    printf( "Grupos secundarios:\n");
     for (int i = 0; i < num_grupos_segundarios; i++) {
        struct group *grupo = getgrgid(groups[i]);
        if(grupo != NULL){
            printf("- %s\n", grupo->gr_name);
        }
    }
}

int builtin_setenv (int argc, char ** argv){
    if(argc!=3){
        fprintf(stderr, "Faltan o sobran argumentos.\n");
        return 1;
    }
    char* variable = argv[1];
    char* valor = argv[2];

    if (setenv(variable, valor, 1) != 0) {
        fprintf(stderr, "Error al definir: %s\n", variable);
        return 1;
    }

    return 0;
}

int builtin_pid(int argc, char **argv) {
    
    printf("Process ID: %d\n", getpid());
    return 0;
}

int builtin_uid (int argc, char ** argv){
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);

    printf("User ID: %d\n", uid);
    printf("Username: %s\n", pwd->pw_name);

    return 0;
}

int builtin_unsetenv (int argc, char ** argv){
    char* variable;
    for(int i = 1; i<argc; i++){
        variable = argv[i];
        if (unsetenv(variable) != 0) {
        fprintf(stderr, "Error al borrar: %s\n", variable);
        return 1;
        }
    }
    return 0;
}


int ejecutar (int argc, char ** argv){
     struct builtin_struct* builtin = builtin_lookup(argv[0]);
        if(builtin!=NULL){
            int comando_interno = (*builtin->func)(argc, argv);
        }else{
            pid_t pid = fork();
            if (pid == -1){
                printf("Error");
                exit(EXIT_FAILURE);
            }
        else if(pid ==0){
            //child process
        }else{
            //parent process
        }
        }
}
int externo (int argc, char ** argv){
    return NULL;
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