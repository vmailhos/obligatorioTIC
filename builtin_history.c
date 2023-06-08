#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minish.h"

//Usa el archivo $HOME/.minish_history (pueden probar crearlo a mano).
//Si el .minish_history no existe, lo crea.
//Muestra 10 comandos y si no hay 10 muestra los que haya. Acepta un valor numérico diferente.
//Mantiene la historia entre llamados al minish.


extern struct deq *history_deq;

int builtin_history (int argc, char ** argv){
   
    struct deq_elem *elem = history_deq->leftmost;

    while (elem != NULL) {
        printf("%s", elem->str);
        elem = elem->next;
    }
    return 0;
}