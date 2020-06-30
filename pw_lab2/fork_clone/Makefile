# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu/ -lpomiar_czasu -lm

# zaleznosci i komendy
fork: fork.o program
	$(LOADER) $(OPT) fork.o -o fork $(LIB)


# jak uzyskac plik fork.o ?
fork.o: fork.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o program
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC)

clone2: clone2.o program
	$(LOADER) $(OPT) clone2.o -o clone2 $(LIB)

# jak uzyskac plik clone.o ?
clone2.o: clone2.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone2.c $(INC)



program:
	$(CCOMP) $(OPT) program.c -o program

clean:
	rm -f *.o
	rm -f program clone fork clone2
