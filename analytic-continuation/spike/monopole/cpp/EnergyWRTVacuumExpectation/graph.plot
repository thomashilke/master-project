set encoding iso_8859_1
set term epslatex color solid size 6.2cm,5cm

set output 'varying_vacuum_f.tex'

set xlabel 'Radius $r$'
set ylabel '$f(r)$'

set xrange [0:20]
set yrange [0:1]

set key bottom right

plot 'tmp_f_g1_v0.2_lambda1.dat' w l title '$v = 0.2$', 'tmp_f_g1_v1.4_lambda1.dat' w l title '$v = 1.4$'


set term epslatex color solid size 6.2cm,5cm
set output 'varying_vacuum_h.tex'

set xlabel 'Radius $r$'
set ylabel '$h(r)$'

set xrange [0:15]
set yrange [0:1]

set key bottom right

plot 'tmp_h_g1_v0.2_lambda1.dat' w l title '$v = 0.2$', \
     'tmp_h_g1_v1.4_lambda1.dat' w l title '$v = 1.4$'



set term epslatex color solid size 12.5cm,5cm
set output 'varying_vacuum_energy.tex'

set xlabel 'Vacuum expectation $v$'
set ylabel 'Total Mass $M(v)$'

set xrange [0.1:1.9]
set yrange [0:35]

plot 'energies.dat' u 2:4 notitle