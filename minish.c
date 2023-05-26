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

int builtin_exit(int argc, char **argv);
int builtin_pid(int argc, char **argv);
int builtin_uid (int argc, char ** argv);


int main(void){ //hay que manejar errores tambien
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    char *username= pwd->pw_name;
    char path[PATH_MAX];    
    char *directorio = getcwd(path, sizeof(path));

    char input[MAXLINE];
    char* argv[MAXLINE];
    int argc;

    while (1){
        fprintf(stderr, "(minish) %s:%s ",username, directorio);
        fgets(input, MAXLINE, stdin);

        //printf("%s: \n", input);

        argc = linea2argv(input, MAXLINE, argv);
        //printf("%s", argv[0]);
        if (argc > 0 && strcmp(argv[0], "exit") == 0) {
            builtin_exit(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "pid") == 0) {
            builtin_pid(argc, argv);
        }
        
        if (argc > 0 && strcmp(argv[0], "uid") == 0) {
            builtin_uid(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "gid") == 0) {
            builtin_gid(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "help") == 0) {
            builtin_help(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "getenv") == 0) {
            builtin_getenv(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "setenv") == 0) {
            builtin_setenv(argc, argv);
        }

        if (argc > 0 && strcmp(argv[0], "unsetenv") == 0) {
            builtin_unsetenv(argc, argv);
        }
    }
    return 0;
    
}