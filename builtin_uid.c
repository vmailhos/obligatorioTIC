#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>

#include "minish.h"
#include "wrappers.h"

int builtin_uid (int argc, char ** argv){
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    //no pueden retornar null? no pueden dar error?
    printf("User ID: %d\n", uid);
    printf("Username: %s\n", pwd->pw_name);

    return 0;
}