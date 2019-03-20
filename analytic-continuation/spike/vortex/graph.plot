set term epslatex color solid size 11cm,5cm

set output 'a_and_f_funcs_vortex.tex'

set xlabel 'Radius $r$'
set ylabel '$A(r)$, $F(r)$'

set xrange [0:7]
set yrange [0:1]

plot 'func_a.m' w l title '$A(r)$', 'func_f.m' w l title '$F(r)$'