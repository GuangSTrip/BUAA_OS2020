all: fibo.c
	gcc -c fibo.c -I ../include
	gcc -c main.c -I ../include
	gcc main.c fibo.o -o ../fibo -I ../include 
clean:
	rm main.o
	rm fibo.o
