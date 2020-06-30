CC = gcc
LD = gcc
OPT = -O3 -fopenmp
LIB = -lm

zad7: ../fib/zad7.o
	$(LD) $(OPT) zad7.o -o zad7 $(LIB)


zad7.o: ../fib/zad7.c
	$(CC) -c $(OPT) zad7.c

clean:
	rm -f *.o
	rm zad7
