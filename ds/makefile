CFLAGS := -ansi -pedantic-errors -Wall -Wextra -c
CSOFLAGS := -shared
CC := gcc
OBJ := $(subst .c,.o,$(filter-out %test.c, $(wildcard **/*.c))) #stack/stack.o
DEP := $(subst .o,.d,$(OBJ)) # uid/uid.d
MAIN := $(subst .c,.out,$(subst ,,$(filter %test.c, $(wildcard **/*.c)))) #stack/stack_test.out
NAME := $(subst .o,,$(OBJ)) # obj/obj
EXCECUT := $(filter test%, $(subst /, test/,$(MAIN))) #test/stack_test.out
T_FILES := $(subst .out,.t,$(MAIN))  #stack/stack_test.t

#all:
#	echo $(subst .d,.o,$(filter %.o, $(subst /, obj/,$(OBJ)))) -----------------------------------------------------
.PHONY: all
all: lib/libds_shared.so $(EXCECUT) relese debug

#test/%_test.out : libds_shared.so test/$(filter-out test%, $(subst _, , $(subst /, ,$@))).c
#	 $(CC) $(subst test/,$(filter-out test%, $(subst _, , $(subst /, ,$@)))/, $(subst .out,.c,$@)) #-lds_shared -o $@

.PHONY: release
release: CFLAGS += -O3
release: lib/libds_shared.so

.PHONY: debug
debug: CFLAGS += -g
debug: lib/libds_shared.so


lib/libds_shared.so: $(filter %.o, $(subst /, obj/,$(OBJ)))
	$(CC) $(CSOFLAGS) $^ -o $@

%.d : %.c
	$(CC) -MM $(CFLAGS) -MT obj/$(subst .d,.o,$(lastword $(subst /, ,$@))) $^ > $@
	echo '\t'$(CC) -c $^ -fPIC -o obj/$(subst .d,.o,$(lastword $(subst /, ,$@))) >> $@
	
-include $(DEP)

%.t : %.c
	echo test/$(lastword $(subst /, ,$(subst .c,.out,$<))) : $< lib/libds_shared.so > $@
	echo '\t'$(CC) $< -lds_shared -L /lib -Wl,-rpath=/lib -o test/$(lastword $(subst /, ,$(subst .c,.out,$<))) >> $@

-include $(T_FILES)

#%.o : %.c
#	$(CC) -c  $(CFLAGS) $< -fPIC

.PHONY: clean
clean:
	rm $(DEP)
	rm $(T_FILES)
	rm obj/*o

