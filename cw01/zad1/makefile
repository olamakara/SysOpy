all:
	make static
	make shared

static:
	gcc -Wall -c library.c
	ar crs liblibrary.a library.o
	rm -f *.o

shared:
	gcc -Wall -c library.c
	gcc -shared -fPIC -o liblibrary.so library.o
	rm -f *.o

clear:
	rm -f *.o
	rm -f *.a
	rm -f *.so
