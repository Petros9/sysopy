CC = gcc -Wall


all: test_all cleanAll

static:
	make cleanAll
	$(CC) -c mylib.c
	ar rcs mylib.a mylib.o

shared:
	make cleanAll
	$(CC) -c -fPIC mylib.c
	$(CC) -shared -fPIC -o mylib.so mylib.o

dynamic:
	make cleanAll
	$(CC) -c -fPIC mylib.c
	$(CC) main.c -o main -L. -ldl -D DLL

clean:
	rm -f *.o

cleanAll:
	rm -f *.o *.a *.so *.ddl

static_3a:
	make cleanAll
	make static
	$(CC) -o main main.c mylib.a -I .
	make tests

shared_3a:
	#new block
	#new block
	#new block
	make cleanAll
	make shared
	make tests
dynamic_3a:
	#new block
	#new block
	#new block
	make cleanAll
	make dynamic
	$(CC) -o main main.c
	make tests
tests:
	make little_diffrence
	make default_diffrence
	make many_files
	make complex

little_diffrence:
	./main initialise_array 1 make_pairs "c.txt:cprim.txt" delete_block 0

default_diffrence:
	./main initialise_array 1 make_pairs "a.txt:b.txt" delete_block 0
huge_diffrence:
	./main initialise_array 1 make_pairs "lamba.txt:beta.txt" delete_block 0
many_files:
	./main initialise_array 3 make_pairs "a.txt:b.txt c.txt:cprim.txt" delete_block 0 

complex:
	./main initialise_array 1 make_pairs "a.txt:b.txt" delete_operation 0 0 delete_block 0


test_all:
	make static_3a >> result.txt
	make shared_3a >> result.txt
	make dynamic_3a >> result.txt
	make cleanAll
