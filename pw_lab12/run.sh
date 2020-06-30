nazwa_pliku=main
#nazwa_obrazu=boulevard.pgm
nazwa_obrazu=casablanca.pgm
#nazwa_obrazu=prosty.pgm

mpicc $nazwa_pliku.c -o $nazwa_pliku.out



#for i in {1..40} ; do
mpirun -np 4 $nazwa_pliku.out $nazwa_obrazu wyn.pgm
#done
