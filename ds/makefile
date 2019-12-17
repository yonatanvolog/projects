CFLAGS = -ansi -pedantic-errors -Wall -Wextra
CC = gcc
SOURCES := $(filter-out %_test.c, $(wildcard */*.c)) #e.g uid/uid.c
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))            #e.g uid/uid.o
OBJ := $(addprefix obj/,$(notdir $(OBJECTS)))		 #e.g obj/uid.o
DEP := $(patsubst %.c,%.d,$(SOURCES))				 #e.g uid/uid.d

EXE := $(patsubst %.o,%.out ,$(OBJECTS))			 #e.g uid/uid.out

.PHONY: all
all: release debug

.PHONY: release
release: CFLAGS += -O3
release: libDS.so

.PHONY: debug
debug: CFLAGS += -g
debug: libDS.so 

.PHONY: test
test: CFLAGS += -g
test: libDS.so $(EXE) 

#%.t:%_test.c
#	%.out:%_test.c libDS.so > $@
#	echo '\t'$(CC) $(CFLAGS) $< -lDS -L. -Wl,-rpath=.. >> $@

%.out:  %_test.o libDS.so
	$(CC) $(CFLAGS) -o test/$(notdir $@) $^ -lDS -L. -Wl,-rpath=..
	
libDS.so: $(OBJ)
	gcc -o libDS.so -shared $(OBJ) $(CFLAGS)

%.d:%.c
	$(CC) -MM -MTobj/$(patsubst %.c,%.o,$(lastword $(subst /, ,$^))) $^ > $@	
	echo '\t'$(CC) $(CFLAGS) -c -fPIC $^ -o  obj/$(patsubst %.c,%.o,$(lastword $(subst /, ,$^))) >> $@
	
-include $(DEP)

.PHONY: clean
clean:
	$(RM) libDS.so **/*.o **/*.out **/*.d 
	
print:
	echo $(SOURCES)


