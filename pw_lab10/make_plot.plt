set ylabel "liczba przeslanych struktur"
set xlabel "t[s]"
set grid
plot "results4.txt" using 1:2 with lines, "results5.txt" using 1:2 with lines, "results5s.txt" using 1:2 with lines, "results5b.txt" using 1:2 with lines lt rgb "red"
plot "results4.txt" using 1:2 with dots, "results5.txt" using 1:2 with dots, "results5s.txt" using 1:2 with dots, "results5b.txt" using 1:2 with dots lt rgb "red"