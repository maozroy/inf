PRG_SUFFIX_FLAG := 0
#
LDFLAGS := 
CFLAGS_INC := 
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g
#
## ==================- NOTHING TO CHANGE BELOW THIS LINE ===================
##
SRCS := $(shell find . -name '*test*')
SOURCES := $(filter-out %_test.c, $(wildcard */*.c))
PRGS := $(patsubst %.c,%,$(SRCS))
PRG_SUFFIX=.exe
BINS := $(patsubst %,%$(PRG_SUFFIX),$(PRGS))
## OBJS are automagically compiled by make.
OBJS := $(patsubst %,%.o,$(PRGS))
##
all : libDS.so $(BINS)
##
libDS.so:
	gcc -o libDS.so -fPIC -shared $(SOURCES)
## For clarity sake we make use of:
.SECONDEXPANSION:
OBJ = $(patsubst %$(PRG_SUFFIX),%.o,$@)
ifeq ($(PRG_SUFFIX_FLAG),0)
	BIN = $(patsubst %$(PRG_SUFFIX),%,$@)
else
 	BIN = $@
endif
## Compile the executables
%$(PRG_SUFFIX) : $(OBJS)	
	$(CC) $(CFLAGS)  $(OBJ) -o $(BIN) -lDS -L. -Wl,-rpath=..
##
## $(OBJS) should be automagically removed right after linking.
##
veryclean:
ifeq ($(PRG_SUFFIX_FLAG),0)
	$(RM) $(PRGS)
else
	$(RM) $(BINS)
endif
##
rebuild: veryclean all
##
## eof Generic_Multi_Main_PWD.makefile

