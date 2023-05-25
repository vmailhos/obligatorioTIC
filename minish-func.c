#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    return NULL;
}
int builtin_gid (int argc, char ** argv){
    return NULL;
}
int builtin_setenv (int argc, char ** argv){
    return NULL;
}
int builtin_pid(int argc, char **argv) {
    
    printf("Process ID: %d\n", getpid());
    return 0;
}
int builtin_uid (int argc, char ** argv){
    return NULL;
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


char* getLinea(){
    char* input = malloc_or_exit(MAXLINE);

    fprintf(stderr, "(nombre1)(algo?)");
    fgets(input, MAXLINE, stdin);

    //printf("You entered: %s\n", input);
    return input;
    //cntD hace que fgets devuelva el fin del archivo
}