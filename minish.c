#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <errno.h>

#include "wrappers.h"
#include "minish.h"

#define HELP_CD      "cd [..|dir] - cambia de directorio corriente"
#define HELP_DIR     "dir [str]- muestra archivos en directorio corriente, que tengan 'str'"
#define HELP_EXIT    "exit [status] - finaliza el minish con un status de retorno (por defecto 0)"
#define HELP_HELP    "help [cd|dir|exit|help|history|getenv|pid|setenv|status|uid]"
#define HELP_HISTORY "history [N] - muestra los últimos N (10) comandos escritos"
#define HELP_GETENV  "getenv var [var] - muestra valor de variable(s) de ambiente"
#define HELP_PID     "pid - muestra Process Id del minish"
#define HELP_SETENV  "setenv var valor - agrega o cambia valor de variable de ambiente"
#define HELP_STATUS  "status - muestra status de retorno de ultimo comando ejecutado"
#define HELP_UID     "uid - muestra nombre y número de usuario dueño del minish"
#define HELP_GID     "gid - muestra grupo principal y secundarios"
#define HELP_UNSETENV   "unsetenv - var [var ...] - elimina variable(s) de ambiente"

#define MAXLINE 1024
#define PATH_MAX 4096

char *directorio = NULL;
struct deq *history_deq;

struct builtin_struct builtin_arr[] = {
    { "exit", builtin_exit, HELP_EXIT },
    { "help", builtin_help, HELP_HELP },
    { "history", builtin_history, HELP_HISTORY },
    { "status", builtin_status, HELP_STATUS },
    { "cd", builtin_cd, HELP_CD },
    { "dir", builtin_dir, HELP_DIR },
    { "getenv", builtin_getenv, HELP_GETENV },
    { "gid", builtin_gid, HELP_GID },
    { "setenv", builtin_setenv, HELP_SETENV },
    { "pid", builtin_pid, HELP_PID },
    { "uid", builtin_uid, HELP_UID },
    { "unsetenv", builtin_unsetenv, HELP_UNSETENV },
    { NULL, NULL, NULL }
};

struct builtin_struct* builtin_lookup(char *cmd){
    int i = 0;
    while(builtin_arr[i].cmd!=NULL){
        if(strcmp(cmd,builtin_arr[i].cmd)==0){
            return &builtin_arr[i];
        }
        i++;
    }
    return NULL; 
}

void sigint_handler(int sig) {
    fprintf(stderr, " : Interrupt! %d\n", sig);
}

void add_to_history(char* command) {
    deq_append(history_deq, command);
}

void save_history(struct deq_elem * structDeq1, struct deq *deque) {
    char filename[FILENAME_MAX];
    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        perror("Error al obtener la variable de entorno HOME\n");
        return;
    }
    snprintf(filename, sizeof(filename), "%s/%s", home_dir, ".minish_history");

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error al abrir archivo de history\n");
        return;
    }

    struct deq_elem *elem = structDeq1;
    if(elem==NULL){
        elem = deque->leftmost;
    }else{
        elem = elem->next;
    }

    while (elem != NULL) {
        fprintf(file, "%s", elem->str);
        elem = elem->next;
    }
    fclose(file);
}

struct deq_elem * load_history() {
    char filename[FILENAME_MAX];
    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        perror("Error al obtener la variable de entorno HOME\n");
        return NULL;
    }
    snprintf(filename, sizeof(filename), "%s/%s", home_dir, ".minish_history");

    struct deq_elem * punteroASesionNueva;
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        //SI DIO NULL PUEDE SER XQ AUN NO FUE CREADO
        file = fopen(filename, "w");
        if(file == NULL){
            //SI DIO NULL HUBO UN ERROR
            perror("Error al abrir archivo de history\n");
            return NULL;
        }
        fclose(file);
        return NULL;
    }

    char line[MAXLINE];

    while (fgets(line, MAXLINE, file) != NULL) {
        punteroASesionNueva = deq_append(history_deq, line);
    }
    fclose(file);
    return punteroASesionNueva;
}




int main(void){ //hay que manejar errores tambien
    struct sigaction str_sigint_action;
    memset(&str_sigint_action, 0, sizeof(str_sigint_action));
    str_sigint_action.sa_handler = sigint_handler;
    if(sigaction(SIGINT, &str_sigint_action, NULL)==-1){
        perror("Error en sigaction\n");
        return 1;
    }

    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if (pwd==NULL){
        perror("Error al encontrar el userID\n");
        return 1; ///no se si tiene sentido porque en el status no lo estarias agarrando
    }
    char *username= pwd->pw_name;


    char input[MAXLINE];
    char* argv[MAXLINE];
    int argc;
    char* respuesta;

    history_deq = deq_create();
    struct deq_elem * punteroAPrimerElDeSesionNueva = load_history();
    if (punteroAPrimerElDeSesionNueva==NULL){
        perror("Error al encontrar el userID\n");
        return 1; ///no se si tiene sentido porque en el status no lo estarias agarrando
    }
   
    //int status=0;

    while (1){ 
        char path[PATH_MAX];
        directorio = getcwd(path, sizeof(path));

        fprintf(stdout, "(minish) %s:%s ",username, directorio);
        clearerr(stdin);
        respuesta = fgets(input, MAXLINE, stdin);
        if(respuesta==NULL){
            if(feof(stdin)){
                save_history(punteroAPrimerElDeSesionNueva, history_deq);
                argv[0] = "exit";
                ejecutar(1, argv);
            }
            continue;
        }

        argc = linea2argv(input, MAXLINE, argv);

        if(argc!=0){
            add_to_history(input);
            if(strcmp(argv[0],"exit")==0){
                save_history(punteroAPrimerElDeSesionNueva, history_deq);
            }
            ejecutar(argc,argv);
        }
    }
   
    return 0;    
}