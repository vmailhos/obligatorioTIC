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
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

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

//Función que busca un comando interno por su nombre y devuelve su estructura
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

//Manejador de la señal SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    fprintf(stderr, " : Interrupt! %d\n", sig);
}

//Agrega un comando al historial
void add_to_history(char* command) {
    deq_append(history_deq, command);
}

//Guarda el historial en un archivo
void save_history(struct deq_elem * structDeq1, struct deq *deque) {
    char filename[FILENAME_MAX];
    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        fprintf(stderr,"Error al obtener la variable de entorno HOME\n");
        return;
    }

    //Construye la ruta completa del archivo de historial
    snprintf(filename, sizeof(filename), "%s/%s", home_dir, ".minish_history");

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr,"Error al abrir archivo de history\n");
        return;
    }

    struct deq_elem *elem = structDeq1;

    // Determina el elemento desde donde comenzar a guardar en el archivo
    if(elem==NULL){
        //Si no se especifica un elemento, se toma el elemento más a la izquierda del deque
        elem = deque->leftmost;
    }else{
        //Si se especifica un elemento, se toma el siguiente elemento
        elem = elem->next;
    }

    //Escribe cada elemento en el archivo
    while (elem != NULL) {
        fprintf(file, "%s", elem->str);
        elem = elem->next;
    }
    fclose(file);
}

//Carga el historial desde un archivo
struct deq_elem * load_history() {
    char filename[FILENAME_MAX];
    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        fprintf(stderr,"Error al obtener la variable de entorno HOME\n");
        return NULL;
    }

    //Construye la ruta completa del archivo de historial
    snprintf(filename, sizeof(filename), "%s/%s", home_dir, ".minish_history");

    struct deq_elem * punteroASesionNueva;
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        //Si es NULL, es posible que el archivo aún no exista: se crea uno nuevo en modo escritura
        file = fopen(filename, "w");
        if(file == NULL){
            //Si no se puede crear el archivo, se muestra un error
            fprintf(stderr,"Error al abrir archivo de history\n");
            return NULL;
        }
        fclose(file);
        return NULL;
    }

    char line[MAXLINE];
    //Lee cada línea del archivo y la agrega al historial
    while (fgets(line, MAXLINE, file) != NULL) {
        punteroASesionNueva = deq_append(history_deq, line);
    }
    fclose(file);
    //Retorna el puntero al primer elemento de la nueva sesión de historial
    return punteroASesionNueva;
}

// Libera la memoria asignada a los argumentos de la línea de comandos
void free_argv(int argc, char* argv[]){
    for(int i=0; i<argc; i++){
        free(argv[i]);
        argv[i] = NULL;
    }
}


int main(void){ 
    struct sigaction str_sigint_action;
    memset(&str_sigint_action, 0, sizeof(str_sigint_action));
    
    //Establece el manejador de señal para SIGINT
    str_sigint_action.sa_handler = sigint_handler;
    if(sigaction(SIGINT, &str_sigint_action, NULL)==-1){
        perror("Error en sigaction\n");
        exit(1);
    }

    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    char *username= pwd->pw_name;


    char input[MAXLINE];
    char* argv[MAXLINE];
    int argc;
    char* respuesta;

    history_deq = deq_create();
    struct deq_elem * punteroAPrimerElDeSesionNueva = load_history();
   
    while (1){ 
        //Limpia los argumentos anteriores
        free_argv(argc, argv);

        // Obtiene el directorio actual
        char path[PATH_MAX];
        directorio = getcwd(path, sizeof(path));

        //Obtiene el directorio actual
        //fprintf(stdout, "(minish) %s:%s ",username, directorio);
        printf("%s(minish) %s:%s %s", COLOR_GREEN, username, directorio, COLOR_RESET);
        //Limpia los errores de entrada estándar
        clearerr(stdin);
        //Lee la entrada del usuario
        respuesta = fgets(input, MAXLINE, stdin);
        
        //Verifica si la entrada es nula (EOF)
        if(respuesta==NULL){
            if(feof(stdin)){
                //Guarda el historial antes de salir
                save_history(punteroAPrimerElDeSesionNueva, history_deq);
                //Ejecuta el comando "exit" para finalizar el minish
                argv[0] = "exit";
                ejecutar(1, argv);
            }
            continue;
        }

        //Convierte la línea de entrada en argumentos separados
        argc = linea2argv(input, MAXLINE, argv);
        //Construye una línea de historial para agregarla al historial
        char lineaHistory[MAXLINE] = "";
        for(int i=0; i< argc; i++){
            strcat(lineaHistory, argv[i]);
            strcat(lineaHistory, " ");
        }
        strcat(lineaHistory, "\n");

        //Verifica si hay argumentos para ejecutar
        if(argc!=0){
            //Agrega la línea al historial
            add_to_history(lineaHistory);
            //Verifica si el comando es "exit" para guardar el historial antes de salir
            if(strcmp(argv[0],"exit")==0){
                save_history(punteroAPrimerElDeSesionNueva, history_deq);
            }
            //Ejecuta el comando con los argumentos proporcionados
            ejecutar(argc,argv);
        }
        
    }
   
    return 0;    
}