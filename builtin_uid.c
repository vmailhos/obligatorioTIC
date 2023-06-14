#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>

#include "minish.h"
#include "wrappers.h"
//Uid: 202320, Name: vmailhos, Info = 'Valentina MAILHOS URIOSTE,,,'

//Función para obtener y mostrar información del usuario actual
int builtin_uid (int argc, char ** argv){
    (void)argc;
    (void)argv;
    
    //Obtener el ID de usuario actual
    uid_t uid = getuid();
    //Obtener la estructura de datos del usuario
    struct passwd *pwd = getpwuid(uid);
    
    //Cuando no se logra obtener los datos del usuario, imprimir mensaje de error 
    if (pwd==NULL){
        perror("Error al encontrar el userID");
        return 1;
    }

    //Imprimir información del usuario
    printf("Uid: %d, Name: %s, Info: %s\n", uid, pwd->pw_name, pwd->pw_gecos);
    return 0;
}