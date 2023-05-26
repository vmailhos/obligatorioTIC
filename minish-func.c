#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include "minish.h"
#include "wrappers.h"
#define IN 1
#define OUT 0
#define MAXLINE 1024

//struct builtin_struct * builtin_lookup(char *cmd);

int last_exit_status = 0;
int builtin_exit (int argc, char ** argv){
    int status = last_exit_status;

    if (argc > 1) {
        status = atoi(argv[1]);
    }

    exit(status);
}

int builtin_help (int argc, char ** argv){
    return NULL;
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
    return NULL;
}
int builtin_setenv (int argc, char ** argv){
    if(argc!=3){
        printf("Faltan o sobran argumentos.\n");
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
    return NULL;
}
int ejecutar (int argc, char ** argv){
    return NULL;
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