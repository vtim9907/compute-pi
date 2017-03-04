reset

set datafile separator ","
set title "compute-pi (500 rounds)"
set xlabel 'N'
set ylabel 'time(sec)'
set key left top
set terminal png size 1000,800 enhanced font 'Verdana,14'
set output 'plot.png'

plot "result_clock_gettime.csv" u 1:2  w lp title "Baseline",\
"result_clock_gettime.csv" u 1:3 w lp title "OpenMP with 2 threads",\
"result_clock_gettime.csv" u 1:4 w lp title "OpenMP with 4 threads",\
 "result_clock_gettime.csv" u 1:5 w lp title "AVX SIMD" ,\
 "result_clock_gettime.csv" u 1:6 w lp  title "AVX SIMD + Loop unrolling" ,\
 "result_clock_gettime.csv" u 1:7 w lp  title "Leibniz OpenMP with 4 threads" ,\
