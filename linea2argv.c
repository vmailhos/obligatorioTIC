#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


#include "minish.h"
#include "wrappers.h"

#define IN 1
#define OUT 0
#define MAXLINE 1024


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



