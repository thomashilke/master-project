set term epslatex color solid size 11cm,5cm

set output 'monopole_funcs_a.eps'

set xlabel '$r$'
set ylabel '$N(r)\equiv 1-f(r)$'

set xrange [0:7]
set yrange [0:1.2]
set ytics (0,0.2,0.4,0.6,0.8,1)

set key top right

plot 'lambda0/cmonopole-r100-l0-3000.dat' u 1:2 w l title '$\lambda=0$',\
     'lambda0.5/cmonopole-r20-l0.5-2000.dat' u 1:2 w l title '$\lambda=0.5$',\
     'lambda1/cmonopole-r20-l1-2000.dat' u 1:2 w l title '$\lambda=1$'


set output 'monopole_funcs_f.eps'

set xlabel '$r$'
set ylabel '$M(r)/r \equiv h(r)$'

set xrange [0:7]
set yrange [0:1.2]

set key bottom right

plot 'lambda0/cmonopole-r100-l0-3000.dat' u 1:($3/$1) w l title '$\lambda=0$',\
     'lambda0.5/cmonopole-r20-l0.5-2000.dat' u 1:($3/$1) w l title '$\lambda=0.5$',\
     'lambda1/cmonopole-r20-l1-2000.dat' u 1:($3/$1) w l title '$\lambda=1$'

set output 'monopole_anal_f_real.eps'

set xlabel '$x$'
set ylabel '$y$'

set ytics autofreq

set xrange [-10:10]
set yrange [0:10]
set zrange [-10:10]
set cbrange [-10.1:10.1]
set cbtics (-10,-5,0,5,10)

set view map
splot 'lambda0.5/monopole-r10-l0.5.dat' u 1:2:3 w pm3d  notitle

set output 'monopole_anal_f_imag.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-10:10]
set cbrange [-10.1:10.1]
set cbtics (-10,-5,0,5,10)

set view map
splot 'lambda0.5/monopole-r10-l0.5.dat' u 1:2:7 w pm3d  notitle

set output 'monopole_anal_h_real.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-5:10]
set cbrange [-5.1:10.1]
set cbtics (-5,0,5,10)

set view map
splot 'lambda0.5/monopole-r10-l0.5.dat' u 1:2:5 w pm3d  notitle

set output 'monopole_anal_h_imag.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-5:10]
set cbrange [-5.1:10.1]
set cbtics (-5,0,5,10)

set view map
splot 'lambda0.5/monopole-r10-l0.5.dat' u 1:2:9 w pm3d  notitle




############## BPS:
set output 'monopole_anal_f_real_bps.eps'

set xlabel '$x$'
set ylabel '$y$'

set ytics autofreq

set xrange [-10:10]
set yrange [0:10]
set zrange [-10:10]
set cbrange [-10.1:10.1]
set cbtics (-10,-5,0,5,10)

set view map
splot 'lambda0/monopole-r10-l0.dat' u 1:2:3 w pm3d  notitle

set output 'monopole_anal_f_imag_bps.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-10:10]
set cbrange [-10.1:10.1]
set cbtics (-10,-5,0,5,10)

set view map
splot 'lambda0/monopole-r10-l0.dat' u 1:2:7 w pm3d  notitle

set output 'monopole_anal_h_real_bps.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-5:10]
set cbrange [-5.1:10.1]
set cbtics (-5,0,5,10)

set view map
splot 'lambda0/monopole-r10-l0.dat' u 1:2:5 w pm3d  notitle

set output 'monopole_anal_h_imag_bps.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-5:10]
set cbrange [-5.1:10.1]
set cbtics (-5,0,5,10)

set view map
splot 'lambda0/monopole-r10-l0.dat' u 1:2:9 w pm3d  notitle