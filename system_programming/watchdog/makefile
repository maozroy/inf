# Makefile for dynamic library creation

C_DEPS= watcher.c wd_utils.c

WD_SO_DEP = watcher.o wd_utils.o

all: watch_test_out watch_dog_lib 

watch_test_out: libwatchdog.so watcher_test.c
	gcc -Wall -o watcher_test.out watcher_test.c watcher.c wd_utils.c -pthread -I/home/student/git/ds/scheduler -I/home/student/git/ds/uid -lm -lwatchdog -lscheduler -L. -Wl,-rpath=.

watch_dog_lib: $(WD_SO_DEP) 
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -shared -Wl,-soname,libwatchdog.so $(WD_SO_DEP) -o libwatchdog.so

create_objs: $(C_DEPS)
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -fPIC -c $(C_DEPS)
