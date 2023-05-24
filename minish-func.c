#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minish.h"
#include "wrappers.h"
#define IN 1
#define OUT 0
#define MAXLINE 1024

//struct builtin_struct * builtin_lookup(char *cmd);

int builtin_exit (int argc, char ** argv){
    return NULL;
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
int builtin_pid (int argc, char ** argv){
    return NULL;
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

int linea2argv(char *linea, int argc, char **argv){
    int i = 0;
    int c;
    int state = OUT;
    int wordcount = 0;
    int j = 0;
    char temp[MAXLINE];
    while((c = linea[i])!='\n'){
        printf("%d -> ", i);
        printf("%c\n", c);
        if(c==' '||c=='\t'){
            temp[j] = '\0';
            if(state == IN){
                argv[wordcount-1] = strdup_or_exit(temp);
                j = 0;
            }
            state = OUT;
        }else if(state == OUT){
            state = IN;
            wordcount++;
        }

        if(state == IN){
            temp[j] = linea[i];
            j++;
        }
        i++;
    }
    
    printf("%d\n",wordcount);
    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);

    return wordcount;
}

/* j = 0;
        while(linea[i]==' '||linea[i]=='\t'){ //se saltea los primeros espacios si hay
            i++; 
        }
        while(linea[i]!=' '||linea[i]!='\t'){ //se saltea los primeros espacios si hay
            texto[j]=linea[i];
            i++;
            j++;
        }
        char* info = strdup_or_exit(texto);
        contador++;
        argv[contador] = info;*/

char *get_word(char *line, int position, char *word_to_print){
    int i=0; 
    int x=0;
    while(line[i]==' '||line[i]=='\t'){ //se saltea los primeros espacios si hay
            i++; 
    }
    for (int j=0; j<position-1; j++){ //se saltea palabras y sus espacios dsp
        while(line[i]!=' ' && line[i]!='\0' && line[i]=='\t'){
            i++;
        }
        while(line[i]==' '&&line[i]!='\0' && line[i]=='\t'){
            i++; 
        }
    }
    while(line[i]!=' ' && line[i]!='\0' && line[i]=='\t'){
        word_to_print[x]=line[i]; 
        i++;
        x++;
    }
    word_to_print[x]='\0';//no se si tiene del todo sentido porq lo cree con un tamaño max
    return word_to_print;
}

