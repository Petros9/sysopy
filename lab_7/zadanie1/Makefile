all:
	make compile_receiver
	make compile_packer
	make compile_sender
	make compile_main
	make run

compile_receiver:
	gcc -o receiver receiver.c -Wall

compile_packer:
	gcc -o packer packer.c -Wall

compile_sender:
	gcc -o sender sender.c -Wall

compile_main:
	gcc -o main main.c -Wall

run:
	./main

remove:
	rm receiver
	rm packer
	rm sender
	rm main
