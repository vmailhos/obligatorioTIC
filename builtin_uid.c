#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>

#include "minish.h"
#include "wrappers.h"

//no puede tener errores
//Uid: 202320, Name: vmailhos, Info = 'Valentina MAILHOS URIOSTE,,,'
int builtin_uid (int argc, char ** argv){
    
    (void)argc;
    (void)argv;
    
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if (pwd==NULL){
        perror("Error al encontrar el userID");
        return 1;
    }
    printf("Uid: %d, Name: %s, Info: %s\n", uid, pwd->pw_name, pwd->pw_gecos);
    return 0;
}