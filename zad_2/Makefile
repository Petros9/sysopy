CC = gcc -Wall


all: static shared clean

static:
	$(CC) -c mylib.c
	ar rcs mylib.a mylib.o

shared:
	$(CC) -c -fPIC mylib.c
	$(CC) -shared -fPIC -o mylib.so mylib.o

clean:
	rm -f *.o
