CC = gcc -Wall

clean:
	rm -f *.o

cleanAll:
	rm -f *.o *.a *.so *.ddl

static_3b:
	make cleanAll
	$(CC) -c mylib.c -O${step}
	ar rcs mylib.a mylib.o
	$(CC) -o main main.c mylib.a -I . -O${step}
	make tests

shared_3b:
	#new block
	#new block
	#new block
	make cleanAll
	make cleanAll
	$(CC) -c -fPIC mylib.c -O${step}
	$(CC) -shared -fPIC -o mylib.so mylib.o -O${step}
	make tests
dynamic_3b:
	#new block
	#new block
	#new block
	make cleanAll
	$(CC) -c -fPIC mylib.c -O${step}
	$(CC) main.c -o main -L. -ldl -D DLL -O${step}
	$(CC) -o main main.c -O${step}
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


all_0:
	make static_3b step=0 >> result.txt
	make shared_3b step=0 >> result.txt
	make dynamic_3b step=0 >> result.txt
	make cleanAll

all_1:
	make static_3b step=1 >> result.txt
	make shared_3b step=1 >> result.txt
	make dynamic_3b step=1 >> result.txt
	make cleanAll

all_2:
	make static_3b step=2 >> result.txt
	make shared_3b step=2 >> result.txt
	make dynamic_3b step=2 >> result.txt
	make cleanAll


all_3:
	make static_3b step=3 >> result.txt
	make shared_3b step=3 >> result.txt
	make dynamic_3b step=3 >> result.txt
	make cleanAll


all_s:
	make static_3b step=s >> result.txt
	make shared_3b step=s >> result.txt
	make dynamic_3b step=s >> result.txt
	make cleanAll


