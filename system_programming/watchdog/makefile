# Makefile for dynamic library creation

C_DEPS= ../../ds/scheduler/libscheduler.so watcher.c wd_utils.c

OBJ_DEPS=watcher.o wd_utils.o

all: create_objs create_shared_lib remove_objs

create_objs: $(C_DEPS)
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -fPIC -c $(C_DEPS) -pthread -I /home/student/git/ds/scheduler

create_shared_lib: $(OBJ_DEPS)
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -shared -Wl,-soname,libwatchdog.so $(OBJ_DEPS) -o libwatchdog.so

remove_objs: $(OBJ_DEPS)
	rm *.o

