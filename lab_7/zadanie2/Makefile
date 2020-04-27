all:
	make compile_receiver
	make compile_packer
	make compile_sender
	make compile_main
	make run

compile_receiver:
	gcc -o receiver receiver.c -Wall -lpthread -lrt

compile_packer:
	gcc -o packer packer.c -Wall -lpthread -lrt

compile_sender:
	gcc -o sender sender.c -Wall -lpthread -lrt

compile_main:
	gcc -o main main.c -Wall -lpthread -lrt

run:
	./main

remove:
	rm receiver
	rm packer
	rm sender
	rm main
