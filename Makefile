# La variable CC indica el ejecutable para el compilador C (usualmente son gcc o clang)
CC=gcc
# La variable CFLAGS indica las banderas para el compilador C
CFLAGS=-Wall -Wextra -std=gnu99 -ggdb
# La variable LDLIBS indica las banderas para la parte del "loader", usualmente librerías "-lxxxx"
LDLIBS=

# Este sería el ejecutable, sus dependencias son los .o
minish:   minish-func.o builtin_exit.o builtin_help.o builtin_history.o builtin_status.o builtin_cd.o builtin_dir.o builtin_getenv.o builtin_gid.o builtin_setenv.o builtin_pid.o builtin_uid.o builtin_unsetenv.o minish.o wrappers-mac.o

# Acá van los .o, make sabe que cada .o depende del .c correspondiente, acá se agrega la dependencia al .h
minish-func.o: minish.h wrappers.h
minish.o: minish.h wrappers.h
#wrappers.o: wrappers.h
wrappers-mac.o: wrappers.h
builtin_exit.o: minish.h wrappers.h
builtin_help.o: minish.h wrappers.h
builtin_history.o: minish.h wrappers.h
builtin_status.o: minish.h wrappers.h
builtin_cd.o: minish.h wrappers.h
builtin_dir.o: minish.h wrappers.h
builtin_getenv.o: minish.h wrappers.h 
builtin_gid.o: minish.h wrappers.h 
builtin_setenv.o: minish.h wrappers.h 
builtin_pid.o: minish.h wrappers.h 
builtin_uid.o: minish.h wrappers.h
builtin_unsetenv.o: minish.h wrappers.h
