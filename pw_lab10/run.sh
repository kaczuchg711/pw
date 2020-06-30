#mpicc zad5b.c -o aaa.out
#for (( i = 2; i <= 2000; i+=10 )); do
#    mpirun -np 2 aaa.out $i
#done

mpicc zad4.c -o aaa.out
for (( i = 2; i <= 1000000; i+=1000 )); do
    mpirun -np 2 aaa.out $i
done

#mpicc zad5s.c -o aaa.out
#for (( i = 2; i <= 2000; i+=10 )); do
#    mpirun -np 2 aaa.out $i
#done
#
#mpicc zad5.c -o aaa.out
#for (( i = 2; i <= 2000; i+=10 )); do
#    mpirun -np 2 aaa.out $i
#done