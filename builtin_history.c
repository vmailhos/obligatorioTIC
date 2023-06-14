#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minish.h"

#define DEFAULT 10

extern struct deq *history_deq;

//Funcion encargada de mostrar las untimas 10 lineas enviadas atravez de la linea de comandos (por defecto).
//Si se acompana la funcion con un numero entonces se mostrara la cantidad de elementos inicados en el mismo.
//Recibe dos argumentos, un array de punteros a char y un int indicando su tamano.
int builtin_history(int argc, char ** argv){
    int cantidad;

    if(argc==1){
        cantidad = DEFAULT;              //por defecto
    }else if (argc==2){
        cantidad = atoi(argv[1]);
    }else{
        fprintf(stderr,"history: más de 1 argumento - help history\n");
        return 1;
    }

    if(cantidad > history_deq->count) cantidad = history_deq->count;
    
    struct deq_elem *elem = history_deq->rightmost;

    for(int i=0; i<cantidad-1; i++){
        elem = elem->prev;
    }

    for(int i=0; i<cantidad; i++){
        printf("%s", elem->str);
        elem = elem->next;
    }

    return 0;
}