reset

set datafile separator ","
set title "compute pi error"
set xlabel 'N'
set ylabel 'error'
set key left top
set terminal png size 1000,800 enhanced font 'Verdana,14'
set output 'pi_error.png'

plot "result_pi_error.csv" u 1:2  w lp title "Baseline error",\
"result_pi_error.csv" u 1:3  w lp title "Leibniz  formula for pi error",\
