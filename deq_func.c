#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minish.h"
#include "wrappers.h"

struct deq *deq_create(void) {
    struct deq *deque;
    deque = (struct deq *) malloc_or_exit(sizeof(struct deq));
    deque->count = 0;
    deque->leftmost = NULL;
    deque->rightmost = NULL;
    return deque;
}

struct deq_elem *deq_append(struct deq *deque, char *s) {
    struct deq_elem *new_elem;
    new_elem = (struct deq_elem *) malloc_or_exit(sizeof(struct deq_elem));
    new_elem->str = malloc_or_exit(strlen(s) + 1);
    strcpy(new_elem->str, s);
    new_elem->next = NULL;

    if (deque->rightmost == NULL) {
        new_elem->prev = NULL;
        deque->leftmost = new_elem;
    } else {
        new_elem->prev = deque->rightmost;
        deque->rightmost->next = new_elem;
    }
        deque->rightmost = new_elem;
    deque->count++;

    return new_elem;
}

void deq_print(struct deq *deque){
    int size = deque->count;
    struct deq_elem* nodo1 = deque->leftmost;
    for(int i=0; i<size; i++){
        printf("Elemento n:%d s:%s", i, nodo1->str);
        nodo1 = nodo1->next;
    }
}