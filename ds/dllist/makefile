.PHONY: all
		
all : dllist.o libdllist.so dllist_test.o dllist_test.out

dllist.o: dllist.c
		gcc -o dllist.o -c -fpic dllist.c

libdllist.so: dllist.o
		gcc -shared dllist.o -o libdllist.so

dllist_test.o: dllist_test.c dllist.h
		gcc -o dllist_test.o -c dllist_test.c

dllist_test.out: dllist_test.o 
		gcc -Wall -o dllist_test.out dllist_test.o -ldllist -L. -Wl,-rpath=/home/student/dllist




