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
#define HELP_GID     "INFO FALTA GID"
#define HELP_UNSETENV   "info falta uset env"

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
    fprintf(stderr, " : ctrlC atrapado: %d\n", sig);
}


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

void add_to_history(char* command) {
    deq_append(history_deq, command);
}

void save_history(struct deq_elem * structDeq1, struct deq *deque) {
    //char history_file[PATH_MAX];

    //snprintf(history_file, sizeof(history_file), "%s/%s", getenv("HOME"), ".minish_history");

    FILE *file = fopen(".history", "a");
    if (file == NULL) {
        perror("Error al abrir archivo de history");
        return;
    }

    struct deq_elem *elem = structDeq1;
    elem = elem->next;
    if(elem==NULL){
        elem  = deque->leftmost;
    }

    while (elem != NULL) {
        fprintf(file, "%s", elem->str);
        elem = elem->next;
    }
    fclose(file);
}

struct deq_elem * load_history() {
    struct deq_elem * punteroASesionNueva;
    FILE *file = fopen(".history", "r");
    if (file == NULL) {
        perror("Error al abrir archivo de history");
        return;
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
    sigaction(SIGINT, &str_sigint_action, NULL);

    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    char *username= pwd->pw_name;


    char input[MAXLINE];
    char* argv[MAXLINE];
    int argc;
    char* respuesta;

    history_deq = deq_create();
    struct deq_elem * punteroAPrimerElDeSesionNueva = load_history();
   
    //int status=0;

    while (1){ 
        char path[PATH_MAX];
        directorio = getcwd(path, sizeof(path));

        fprintf(stdout, "(minish) %s:%s ",username, directorio);
        clearerr(stdin);
        respuesta = fgets(input, MAXLINE, stdin);
        if(respuesta==NULL){
            if(feof(stdin)){
                break;
            }
            continue;
        }
        add_to_history(input);
        argc = linea2argv(input, MAXLINE, argv);

        if(strcmp(argv[0],"exit")==0){
            save_history(punteroAPrimerElDeSesionNueva, history_deq);
        }

        if(argc!=0){
            //status=ejecutar(argc,argv);
            ejecutar(argc,argv);
        }
    }
   
    return 0;    
}