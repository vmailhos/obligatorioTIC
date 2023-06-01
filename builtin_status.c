#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int statusValue; 

int builtin_status (void){
    printf("%d\n", statusValue); 
    return 0; 
}

