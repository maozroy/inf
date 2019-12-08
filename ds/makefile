CFLAGS := -ansi -pedantic-errors -Wall -Wextra -g
CSOFLAGS := -shared
CC := gcc
SOURCES := $(subst .c,.o,$(filter-out %test.c, $(wildcard */*.c)))
MAIN := $(subst .c,.o,$(filter %test.c, $(wildcard */*.c)))
NAME := $(subst .c,,$(SOURCES))

all:
	echo $(SOURCES)
	
#executable: $(NAME)_test.o lib$(NAME)_shared.so
#	$(CC) $(CFLAGS) $(NAME)_test.o -l$(NAME)_shared -o $(NAME).out


#lib$(NAME)_shared.so: $(NAME).o
#	$(CC) -shared $(NAME).o -o /usr/lib/lib$(NAME)_shared.so

#$(NAME).o: $(NAME).c $(NAME).h
#	cc -c $(NAME).c $(NAME).h -fPIC

#$(NAME)_test.o: $(NAME)_test.c $(NAME).h
#	cc -c $(NAME)_test.c $(NAME).h -fPIC
	
.PHONY: clean
clean:
	rm *.o
