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
        // Si el carácter actual es un espacio o una tabulació
        if (c == ' ' || c == '\t') {
            if (state == IN) {
                temp[j] = '\0';
                //Almacena la palabra en argv
                argv[wordcount] = strdup_or_exit(temp);
                
                //Incrementa el contador de palabras
                wordcount++;
                j = 0;  //Reinicia el índice del temporizador
                //Cambia el estado a OUT (fuera de una palabra)
                state = OUT;
            }
        // Si el carácter actual no es un espacio o una tabulación
        } else {
            if (state == OUT) {
                //Cambia el estado a IN (dentro de una palabra)
                state = IN;
            }
            //Almacena el carácter en el temporizador
            temp[j] = linea[i];
            j++;    //Incrementa el índice del temporizador
        }
        //Incrementa el índice de la línea
        i++;
    }

    if (state == IN) {
        temp[j] = '\0';
        //Almacena la última palabra en argv
        argv[wordcount] = strdup_or_exit(temp);
        wordcount++;    //Incrementa el contador de palabras
    }

    if (wordcount < argc) {
        //Establece el último elemento de argv a NULL
        argv[wordcount] = NULL;
    }
    //Devuelve el número de palabras encontradas
    return wordcount;
}



