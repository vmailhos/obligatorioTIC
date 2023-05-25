#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "wrappers.h"
#include "minish.h"

#define MAXLINE 1024
#define PATH_MAX 4096

int main(void){ //hay que manejar errores tambien
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    char *username= pwd->pw_name;
    char path[PATH_MAX];    
    char *directorio = getcwd(path, sizeof(path));

    char input[MAXLINE];
    while (1){
        fprintf(stderr, "(minish) %s:%s ",username, directorio);
        fgets(input, MAXLINE, stdin);
        printf("%s: \n", input);
    }
}