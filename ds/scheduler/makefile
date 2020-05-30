# Makefile for dynamic library creation

C_DEPS=scheduler.c ../task/task.c ../pq/pq.c ../uid/uid.c \
../heap/heap.c ../heap/heapify.c ../vector/vector.c

OBJ_DEPS=scheduler.o task.o pq.o uid.o vector.o heapify.o heap.o

all: create_objs create_shared_lib remove_objs

create_objs: $(C_DEPS)
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -fPIC -c $(C_DEPS)

create_shared_lib: $(OBJ_DEPS)
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -shared -Wl,-soname,libscheduler.so $(OBJ_DEPS) -o libscheduler.so

remove_objs: $(OBJ_DEPS)
	rm *.o

scheduler
