#!/usr/bin/gnuplot

reset

unset key
set xtics nomirror
set ytics nomirror
set border front

set terminal png size 800,600
set output "../html/plot.png"

plot "result/best.dat" with points pt 7 ps 0.5
