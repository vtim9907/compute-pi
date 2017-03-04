reset

set datafile separator ","
set title "compute pi error"
set xlabel 'N'
set ylabel 'error'
set key left top
set terminal png size 1000,800 enhanced font 'Verdana,14'
set output 'pi_berror.png'

plot "result_pi_base_error.csv" u 1:2  w lp title "Baseline error",\
