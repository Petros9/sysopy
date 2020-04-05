all:
	make compile
	make test
	make delete
compile:
	gcc -o sorter sorter.c -Wall
test:
	./sorter tekst.txt
delete:
	rm sorter