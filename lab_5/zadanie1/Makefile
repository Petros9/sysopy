all:
	make compile
	make test
	make delete 

compile:
	gcc -o interpreter interpreter.c -Wall

test:
	./interpreter komendy.txt

delete:
	rm interpreter

