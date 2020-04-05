all:
	make compile
	make test
	make delete

compile:
	gcc -o consumer consumer.c -Wall
	gcc -o producer producer.c -Wall
	gcc -o main main.c -Wall

test:
	./main

delete:
	rm main
	rm consumer
	rm producer
	rm pipe