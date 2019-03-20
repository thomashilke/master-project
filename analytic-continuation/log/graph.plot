set term epslatex color solid size 5cm,5cm header "\\scriptsize"

set output 'log_riemann_real.eps'

set xlabel '$x$' offset -1,-1
set ylabel '$y$' offset 1,-1
set zlabel '$\Re\log(x+iy)$' offset -2,-2 rotate by 90

set xrange [-1.1:1.1]
set yrange [-1.1:1.1]
set zrange [-1.1:1.1]
set cbrange [-1:1]

set xtics (-1,0,1)
set ytics (-1,0,1)
set ztics (-1,0,1)
set cbtics (-1,0,1)

set pm3d depthorder
set view 70,45
splot 'log.dat' u 1:2:3 w pm3d notitle


set output 'log_riemann_imag.eps'

set xlabel '$x$' offset -1,-1
set ylabel '$y$' offset 1,-1
set zlabel '$\Im\log(x+iy)$' offset -2,-2 rotate by 90

set xrange [-1.1:1.1]
set yrange [-1.1:1.1]
set zrange [-1.1:1.1]
set cbrange [0:15]

set xtics (-1,0,1)
set ytics (-1,0,1)
set ztics (0,5,10,15)
set cbtics (0,7.5,15)

set xrange [-1:1]
set yrange [-1:1]
set zrange [-0.1:15.1]

set pm3d depthorder
set view 70,45
splot 'log.dat' u 1:2:4 w pm3d notitle