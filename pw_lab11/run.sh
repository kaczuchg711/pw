#nazwa_pliku='zad2'
#
#mpicc $nazwa_pliku.c -o $nazwa_pliku.out
#mpirun -np 4 $nazwa_pliku.out $1


nazwa_pliku='zad4'

mpicc $nazwa_pliku.c -o $nazwa_pliku.out -lm
mpirun -np 10 $nazwa_pliku.out