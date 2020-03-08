CC = gcc -Wall -std=c11 -ggdb


all: static shared clean

static:
	$(CC) -c mylib.c
	ar rcs mylib.a mlib.o

shared:
	$(CC) -c -fPIC mylib.c
	$(CC) -shared -fPIC -o mylib.so mylib.o

clean:
	rm -f *.o