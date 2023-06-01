#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>

#include "minish.h"
#include "wrappers.h"

//no puede tener errores
//Uid: 202320, Name: vmailhos, Info = 'Valentina MAILHOS URIOSTE,,,'
int builtin_uid (int argc, char ** argv){
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    printf("Uid: %d, Name: %s, Info: %s\n", uid, pwd->pw_name, pwd->pw_gecos);
    return 0;
}