CC=gcc -Wall
LIB=library
TEST=echo init 100; echo count main.c; echo count library.c; echo count library.h; echo count makefile; echo show 0; echo show 1; echo show 2; echo show 3; echo delete 0; echo delete 0; echo delete 0; echo delete 0; echo destroy

all:
	make static
	make shared

static:
	$(CC) -c $(LIB).c
	ar crs lib$(LIB).a $(LIB).o
	rm -f *.o

compileStatic:
	make static
	$(CC) -o main_static main.c lib$(LIB).a

shared:
	$(CC) -c $(LIB).c
	gcc -shared -fPIC -o lib$(LIB).so $(LIB).o
	rm -f *.o

compileShared:
	make shared
	$(CC) -o main_shared main.c -L./ -l$(LIB)

compileDynamic:
	make shared
	$(CC) -o main_dynamic main.c -L./ -ldl -D DLL

runStatic:
	make compileStatic
	./main_static

runShared:
	make compileShared
	LD_LIBRARY_PATH=./ ./main_shared

runDynamic:
	make compileDynamic
	LD_LIBRARY_PATH=./ ./main_dynamic

testStatic:
	make compileStatic
	($(TEST)) | ./main_static >> results_static.txt

testShared:
	make compileShared
	($(TEST)) | (LD_LIBRARY_PATH=./ ./main_shared) >> results_shared.txt

testDynamic:
	make compileDynamic
	($(TEST)) | (LD_LIBRARY_PATH=./ ./main_dynamic) >> results_dynamic.txt

clear:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.txt
	rm -f main_static
	rm -f main_shared
	rm -f main_dynamic