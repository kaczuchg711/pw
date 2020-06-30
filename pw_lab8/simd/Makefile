CC = gcc
LD = gcc
OPT = -O3 -fopenmp
LIB = -lm

zad6: zad6.o
	$(LD) $(OPT) zad6.o -o zad6 $(LIB)


zad7.o: zad6.c
	$(CC) -c $(OPT) zad6.c

clean:
	rm -f *.o
	rm zad6
