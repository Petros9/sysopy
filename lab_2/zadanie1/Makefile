CC = gcc -Wall

all: compile test clean

compile:
	$(CC) -o main main.c


test:

	#sekcja 1 bajta
	touch test1_kopia_1.txt
	touch test1_kopia_2.txt	
		# generacja
		./main generate test1_1.txt 100 1 generate test1_2.txt 1000 1
		# kopiowanie
		./main copy_sys test1_1.txt test1_kopia_1.txt 100 1
		./main copy_lib test1_2.txt test1_kopia_2.txt 1000 1		
		#sortowanie
		./main sort_sys test1_kopia_1.txt 100 1
		./main sort_sys test1_kopia_2.txt 1000 1
		./main sort_lib test1_1.txt 100 1
		./main sort_lib test1_2.txt 1000 1				
	#sekcja 4 bajtów
	touch test4_kopia_1.txt
	touch test4_kopia_2.txt	
		# generacja
		./main generate test4_1.txt 1000 4 generate test4_2.txt 10000 4
		# kopiowanie
		./main copy_sys test4_1.txt test4_kopia_1.txt 1000 4
		./main copy_lib test4_2.txt test4_kopia_2.txt 10000 4		
		#sortowanie
		./main sort_sys test4_kopia_1.txt 1000 4
		./main sort_sys test4_kopia_2.txt 10000 4
		./main sort_lib test4_1.txt 1000 4
		./main sort_lib test4_2.txt 10000 4	
		

	#sekcja 512 bajtów
	touch test512_kopia_1.txt
	touch test512_kopia_2.txt
		# generacja
		./main generate test512_1.txt 300 512 generate test512_2.txt 1000 512
		# kopiowanie
		./main copy_sys test512_1.txt test512_kopia_1.txt 300 512
		./main copy_lib test512_2.txt test512_kopia_2.txt 1000 512		
		#sortowanie
		./main sort_sys test512_kopia_1.txt 300 512
		./main sort_sys test512_kopia_2.txt 1000 512
		./main sort_lib test512_1.txt 300 512
		./main sort_lib test512_2.txt 1000 512


	#sekcja 1024 bajtów
	touch test1024_kopia_1.txt
	touch test1024_kopia_2.txt
		# generacja
		./main generate test1024_1.txt 1000 1024 generate test1024_2.txt 5000 1024
		# kopiowanie
		./main copy_sys test1_1.txt test1_kopia_1.txt 1000 1024
		./main copy_lib test1_2.txt test1_kopia_2.txt 5000 1024		
		#sortowanie
		./main sort_sys test1_kopia_1.txt 1000 1024
		./main sort_sys test1_kopia_2.txt 5000 1024
		./main sort_lib test1_1.txt 1000 1024
		./main sort_lib test1_2.txt 5000 1024	


	#sekcja 4096 bajtów
	touch test4096_kopia_1.txt
	touch test4096_kopia_2.txt
		# generacja
		./main generate test4096_1.txt 2000 4096 generate test4096_2.txt 1000 4096
		# kopiowanie
		./main copy_sys test1_1.txt test1_kopia_1.txt 2000 4096
		./main copy_lib test1_2.txt test1_kopia_2.txt 1000 4096	
		#sortowanie
		./main sort_sys test1_kopia_1.txt 2000 4096
		./main sort_sys test1_kopia_2.txt 1000 4096
		./main sort_lib test1_1.txt 2000 4096
		./main sort_lib test1_2.txt 1000 4096	

	#sekcja 8192 bajtów 
	touch test8192_kopia_1.txt
	touch test8192_kopia_2.txt
		# generacja
		./main generate test8192_1.txt 10000 8192 generate test8192_2.txt 7000 8192
		# kopiowanie
		./main copy_sys test8192_1.txt test8192_kopia_1.txt 10000 8192
		./main copy_lib test8192_2.txt test8192_kopia_2.txt 7000 8192		
		#sortowanie
		./main sort_sys test1_kopia_1.txt 10000 8192
		./main sort_sys test1_kopia_2.txt 7000 8192
		./main sort_lib test1_1.txt 10000 8192
		./main sort_lib test1_2.txt 7000 8192	

clean:
	rm main