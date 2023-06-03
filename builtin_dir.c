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

int builtin_dir (int argc, char ** argv){
    if(argc>3){
        perror("Error, cantidad de argumentos incorrecta\n");
        return 1;
    }

    DIR *carpeta;
    if(argc==1){
        carpeta = opendir(".");
    }else if (argc==2){
        carpeta = opendir(argv[1]);
    }else{
        // falta contemplar ulrimo caso
    }
    
    if (carpeta == NULL) {
        perror("No se pudo abrir la carpeta.\n");
        return 1;
    }

    struct dirent *entrada;
    struct stat statusCarpeta;

    while ((entrada = readdir(carpeta)) != NULL) {
        //consigue la informacion del archivo
        if(stat(entrada->d_name, &statusCarpeta)==-1){
            perror("Error obteniendo informacion del archivo\n");
            return 1;
        }

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
        printf("\t%lld\t%s  %s\n", (long long)size, tiempo_ultima_modificacion,entrada->d_name);
    }
    return 0;
}