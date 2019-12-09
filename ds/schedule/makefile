CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g
LDFLAGS = -fPIC -shared
SOURCES = $(filter-out %_test.c, $(wildcard ../*/*.c))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
LIBPATH = -l_ds -L. -Wl,-rpath=.

.PHONY: all clean
		
all : a.out lib_ds.so

a.out: scheduler_test.o lib_ds.so
	$(CC) $(CFLAGS) -o $@ $< $(LIBPATH)
	make clean

lib_ds.so: $(OBJECTS)
	$(CC) -o lib_ds.so $(LDFLAGS) *.o

%.o:%.c
	$(CC) -c -fpic $^

clean:
	$(RM) *.o

veryclean:
	$(RM) scheduler *.o *.so *.out

test:
	echo $(SOURCES)

test2:
	echo $(OBJECTS)
