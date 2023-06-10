#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"

//Usa el archivo $HOME/.minish_history (pueden probar crearlo a mano).
//Si el .minish_history no existe, lo crea.
//Muestra 10 comandos y si no hay 10 muestra los que haya. Acepta un valor numérico diferente.
//Mantiene la historia entre llamados al minish.


extern struct deq *history_deq;

int builtin_history(int argc, char ** argv){
    int cantidad;

    if(argc==1){
        cantidad = 10;
    }else if (argc==2){
        cantidad = atoi(argv[1]);                           //puede tirar error?
    }else{
        perror("Cantidad de argumentos erronea\n");
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