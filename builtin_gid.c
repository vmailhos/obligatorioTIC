#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <limits.h>



#include "minish.h"
#include "wrappers.h"

int builtin_gid (int argc, char ** argv){
    gid_t gid_principal = getegid();
    //printf("%d", gid_principal);
    
    struct group *grupo_principal = getgrgid(gid_principal);
    if(grupo_principal == NULL){
        return EXIT_FAILURE;
    }
    printf("Grupo Principal: %s\n", grupo_principal->gr_name);

    gid_t groups[NGROUPS_MAX];
    int num_grupos_segundarios = getgroups(NGROUPS_MAX, groups);
    //printf("%d\n", num_grupos_segundarios);
    if(num_grupos_segundarios == -1){
        return EXIT_FAILURE;
    }

    printf( "Grupos secundarios:\n");
     for (int i = 0; i < num_grupos_segundarios; i++) {
        struct group *grupo = getgrgid(groups[i]);
        if(grupo != NULL){
            printf("- %s\n", grupo->gr_name);
        }
    }
}