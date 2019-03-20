set term epslatex color solid size 11cm,5cm header "\\scriptsize"

set output 'vortex_func_a.eps'

set xlabel '$r$'
set ylabel '$A(f)$' offset 1,0

set yrange [0:1.2]
set ytics (0,0.2,0.4,0.6,0.8,1)
set key bottom right

plot 'lambda0.5/cvortex-r10-l0.5-400.dat' u 1:2 w l title '$\lambda=0.5$',\
     'lambda1/cvortex-r10-l1-400.dat' u 1:2 w l title '$\lambda=1$',\
     'lambda2/cvortex-r10-l2-400.dat' u 1:2 w l title '$\lambda=2$'


set output 'vortex_func_f.eps'

set xlabel '$r$'
set ylabel '$F(r)$' offset 1,0

set yrange [0:1.2]
set ytics (0,0.2,0.4,0.6,0.8,1)
set key bottom right

plot 'lambda0.5/cvortex-r10-l0.5-400.dat' u 1:3 w l title '$\lambda=0.5$',\
     'lambda1/cvortex-r10-l1-400.dat' u 1:3 w l title '$\lambda=1$',\
     'lambda2/cvortex-r10-l2-400.dat' u 1:3 w l title '$\lambda=2$'

set output 'vortex_anal_a_real.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set xtics (-10,-5,0,5,10)
set yrange [0:10]
set ytics (-10,-5,0,5,10)
set zrange [-5.1:5.1]
set cbrange [-5.1:5.1]
set cbtics (-5,0,5)

set view map
splot 'lambda1/vortex-r10-l1.dat' u 1:2:3 w pm3d  notitle

set output 'vortex_anal_a_imag.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-2.1:2.1]
set cbrange [-2.1:2.1]
set cbtics (-2,0,2)

set view map
splot 'lambda1/vortex-r10-l1.dat' u 1:2:7 w pm3d  notitle

set output 'vortex_anal_f_real.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-2.1:2.1]
set cbrange [-2.1:2.1]
set cbtics (-2,0,2)

set view map
splot 'lambda1/vortex-r10-l1.dat' u 1:2:5 w pm3d  notitle

set output 'vortex_anal_f_imag.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-10:10]
set yrange [0:10]
set zrange [-5.1:5.1]
set cbrange [-5.1:5.1]
set cbtics (-5,0,5)

set view map
splot 'lambda1/vortex-r10-l1.dat' u 1:2:9 w pm3d  notitle
