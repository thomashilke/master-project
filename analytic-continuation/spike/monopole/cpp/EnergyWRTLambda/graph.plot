set encoding iso_8859_1
set term epslatex color solid size 6.2cm,5cm

set output 'varying_lambda_f.tex'

set xlabel 'Radius $r$'
set ylabel '$f(r)$'

set xrange [0:10]
set yrange [0:1]

set key bottom right

plot 'tmp_f_g1_v1_lambda0.dat' w l title '$\lambda = 0$', \
     'tmp_f_g1_v1_lambda0.05.dat' w l title '$\lambda = 0.05$', \
     'tmp_f_g1_v1_lambda1.dat' w l title '$\lambda = 1$'


set term epslatex color solid size 6.2cm,5cm
set output 'varying_lambda_h.tex'

set xlabel 'Radius $r$'
set ylabel '$h(r)$'

set xrange [0:15]
set yrange [0:1]

set key bottom right

plot 'tmp_h_g1_v1_lambda0.dat' w l title '$\lambda = 0$', \
     'tmp_h_g1_v1_lambda0.05.dat' w l title '$\lambda = 0.05$', \
     'tmp_h_g1_v1_lambda1.dat' w l title '$\lambda = 1$'



set term epslatex color solid size 12.5cm,5cm
set output 'varying_lambda_energy.tex'

set xlabel 'Higgs coupling $\lambda$'
set ylabel 'Total Mass $M$'

set xrange [-0.2:2]
set yrange [12:17]

plot 'energies.dat' u 3:4 notitle
