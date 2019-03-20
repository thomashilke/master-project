set term epslatex color solid size 11cm,5cm

set output 'f_and_h_funcs_monopole.tex'

set xlabel 'Radius $r$'
set ylabel '$f(r)$, $h(r)$'

set xrange [0:10]
set yrange [0:1]

plot 'func_f.m' w l title '$f(r)$', 'func_h.m' w l title '$h(r)$'