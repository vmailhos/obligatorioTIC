#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include "minish.h"
#include "wrappers.h"

void printPermisos(mode_t permisos) {
    printf((permisos & S_IRUSR) ? "r" : "-");
    printf((permisos & S_IWUSR) ? "w" : "-");
    printf((permisos & S_IXUSR) ? "x" : "-");
    printf((permisos & S_IRGRP) ? "r" : "-");
    printf((permisos & S_IWGRP) ? "w" : "-");
    printf((permisos & S_IXGRP) ? "x" : "-");
    printf((permisos & S_IROTH) ? "r" : "-");
    printf((permisos & S_IWOTH) ? "w" : "-");
    printf((permisos & S_IXOTH) ? "x" : "-");
}

struct FileInfo {
    char* filename;
    struct stat* filestat;
};


void bubble_sort(struct FileInfo* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (strcmp(arr[j].filename, arr[j + 1].filename) > 0) {
                // Swap elements if they are out of order
                struct FileInfo temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int printEntradas(DIR *carpeta){
    struct dirent *entrada;
    struct stat statusCarpeta;

    // Array para guardar la informacion del directorio y el nombre
    struct FileInfo* file_info = NULL;
    int num_files = 0; //no se decrementa pq sino en el for estoy bajando el limite y no te imprime todo

    while ((entrada = readdir(carpeta)) != NULL) {
        //consigue la informacion del archivo
        if(fstatat(dirfd(carpeta), entrada->d_name, &statusCarpeta, 0)==-1){
            perror("Error obteniendo informacion del archivo\n");
            return 1;
        }

        // Se crea un FileInfo y guarda la info
        struct FileInfo* info = malloc_or_exit(sizeof(struct FileInfo));

        // se guardan punteroes
        info->filename = strdup_or_exit(entrada->d_name);
        info->filestat = malloc_or_exit(sizeof(struct stat));
        memcpy(info->filestat, &statusCarpeta, sizeof(struct stat));

        // Se agrega la estructura al array
        file_info = realloc(file_info, (num_files + 1) * sizeof(struct FileInfo));
        file_info[num_files] = *info;
        num_files++;

        // Se libera 
        free(info);
    }
        
    // Se hace bubblesort del array
    bubble_sort(file_info, num_files);
// Se imprime la informacion necesaria
    for (int i = 0; i < num_files; i++) {
                
        statusCarpeta=*file_info[i].filestat;

        mode_t permisos =  statusCarpeta.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);   //Get permisos
        off_t size = statusCarpeta.st_size;                                         //Get size archivo
        char *tiempo_ultima_modificacion = ctime(&statusCarpeta.st_mtime);          //Get tiempo ultima modificacion
        tiempo_ultima_modificacion[strlen(tiempo_ultima_modificacion)-1] = '\0';

        // Get nombre del dueno
        struct passwd *pw = getpwuid(statusCarpeta.st_uid);
        if (pw == NULL) {
            perror("Error al obtener el dueno de la carpeta");
            return 1;
        }

        // Get nombre del grupo
        struct group *gr = getgrgid(statusCarpeta.st_gid);
        if (gr == NULL) {
            perror("Error al obtener el grupo de la carpeta");
            return 1;
        } 

        printPermisos(permisos);
        printf("  %ld  %s\t%s", (long) statusCarpeta.st_nlink, pw->pw_name, gr->gr_name);
        printf("\t%lld\t%s  %s\n", (long long)size, tiempo_ultima_modificacion,file_info[i].filename);

        free(file_info[i].filename);
        free(file_info[i].filestat);
    }

    // se libera file_info array
    free(file_info);

    return 0;
}

int filtros(DIR* carpeta, char* filtro) {
    struct dirent* entrada;
    struct stat statusCarpeta;

    // Array para guardar la informacion del directorio y el nombre
    struct FileInfo* file_info = NULL;
    int num_files = 0;

    while ((entrada = readdir(carpeta)) != NULL) {
        // Agarrar la informacion
        if (fstatat(dirfd(carpeta), entrada->d_name, &statusCarpeta, 0) == -1) {
            perror("Error obteniendo informacion del archivo\n");
            return 1;
        }

        char* resultado = strstr(entrada->d_name, filtro);
        // Si el string esta en el nombre
        if (resultado != NULL) {
            // Se crea un FileInfo y guarda la info
            struct FileInfo* info = malloc_or_exit(sizeof(struct FileInfo));

            // se guardan punteroes
            info->filename = strdup_or_exit(entrada->d_name);
            info->filestat = malloc_or_exit(sizeof(struct stat));
            memcpy(info->filestat, &statusCarpeta, sizeof(struct stat));

            // Se agrega la estructura al array
            file_info = realloc(file_info, (num_files + 1) * sizeof(struct FileInfo));
            file_info[num_files] = *info;
            num_files++;

            // Se libera 
            free(info);
        }
    }
    // Se hace bubblesort del array
    bubble_sort(file_info, num_files);

    // Se imprime la informacion necesaria
    for (int i = 0; i < num_files; i++) {
                
        statusCarpeta=*file_info[i].filestat;

        mode_t permisos =  statusCarpeta.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);   //Get permisos
        off_t size = statusCarpeta.st_size;                                         //Get size archivo
        char *tiempo_ultima_modificacion = ctime(&statusCarpeta.st_mtime);          //Get tiempo ultima modificacion
        tiempo_ultima_modificacion[strlen(tiempo_ultima_modificacion)-1] = '\0';

        // Get nombre del dueno
        struct passwd *pw = getpwuid(statusCarpeta.st_uid);
        if (pw == NULL) {
            perror("Error al obtener el dueno de la carpeta");
            return 1;
        }

        // Get nombre del grupo
        struct group *gr = getgrgid(statusCarpeta.st_gid);
        if (gr == NULL) {
            perror("Error al obtener el grupo de la carpeta");
            return 1;
        } 

        printPermisos(permisos);
        printf("  %ld  %s\t%s", (long) statusCarpeta.st_nlink, pw->pw_name, gr->gr_name);
        printf("\t%lld\t%s  %s\n", (long long)size, tiempo_ultima_modificacion,file_info[i].filename);

        free(file_info[i].filename);
        free(file_info[i].filestat);
    }
    free(file_info); // se libera file_info array

    return 0;
}
int builtin_dir (int argc, char ** argv){
    if(argc>3){
        perror("dir: sobran argumentos incorrectos - help dir para más ayuda\n");
        return 1;
    }

    DIR *carpeta;
    if(argc==1){
        carpeta = opendir(".");
        if (carpeta != NULL) {
            return printEntradas(carpeta);
        }else{      
            perror("Error al abrir carpeta\n");
            return 1;
        }
    }else if (argc==2){
        carpeta = opendir(argv[1]);
        if (carpeta != NULL) {
            return printEntradas(carpeta);
        }else{
            carpeta = opendir(".");
            return filtros(carpeta, argv[1]);
        }
    }else{
        carpeta = opendir(argv[1]);
        if (carpeta == NULL) return 1;
        return filtros(carpeta, argv[2]);
    }
    return 0;
}