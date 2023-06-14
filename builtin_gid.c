#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <limits.h>
#include <errno.h>

#include "minish.h"
#include "wrappers.h"

// Función para imprimir información sobre el GID (ID de grupo)
int builtin_gid (int argc, char ** argv){

    (void)argc;
    (void)argv;

    gid_t gid_principal = getegid();
    //printf("%d", gid_principal);
    
    struct group *grupo_principal = getgrgid(gid_principal);
    //Error al encontrar el grupo principal
    if(grupo_principal == NULL){
        perror("Error, no se pudo encontrar el grupo principal"); //nos tira undefined error: 0
        return 1;
    }
    printf("Real Gid: %d (%s)\n", grupo_principal->gr_gid ,grupo_principal->gr_name);

    gid_t groups[NGROUPS_MAX];
    int num_grupos_segundarios = getgroups(NGROUPS_MAX, groups);
    //Error al encontrar los grupos secundarios
    if(num_grupos_segundarios == -1){
        perror("Error, no se pudo encontrar los grupos secundarios"); //nos tira undefined error: 0
        return 1;    }
    
    //Impresion de los grupos secundarios
    printf( "Groups:");
     for (int i = 0; i < num_grupos_segundarios; i++) {
        struct group *grupo = getgrgid(groups[i]);
        if(grupo != NULL){
            if(i == num_grupos_segundarios-1){
                printf(" %d (%s)", grupo_principal->gr_gid, grupo->gr_name);
            }else{
                printf(" %d (%s),", grupo_principal->gr_gid, grupo->gr_name);
            }
        }
    }
    printf("\n");
    return 0;
}