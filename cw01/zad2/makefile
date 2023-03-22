all:
	make static
	make shared

static:
	gcc -Wall -c library.c
	ar crs liblibrary.a library.o
	rm -f *.o

compileStatic:
	make static
	gcc -Wall -o main_static main.c liblibrary.a
	make runStatic

shared:
	gcc -Wall -c library.c
	gcc -shared -fPIC -o liblibrary.so library.o
	rm -f *.o

compileShared:
	make shared
	gcc -Wall -o main_shared main.c -L./ -llibrary
	make runShared

compileDynamic:
	make shared
	gcc -Wall -o main_dynamic main.c -L./ -ldl -D DLL
	make runDynamic

clear:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.txt
	rm -f main_static
	rm -f main_shared
	rm -f main_dynamic

runStatic:
	./main_static

runShared:
	LD_LIBRARY_PATH=./ ./main_shared

runDynamic:
	LD_LIBRARY_PATH=./ ./main_dynamic
