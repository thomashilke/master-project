set term epslatex color solid size 11cm,5cm

set output 'sine_real.eps'

set xlabel '$x$' offset 0,0
set ylabel '$y$' offset 0,0

set xrange [-3*pi:3*pi]
set yrange [0:3*pi]
set zrange [0-0.5:4*pi+0.5]
set cbrange [0:4*pi]

set xtics (-9,-6,-3,0,3,6,9)
set ytics (0,3,6,9)
unset ztics 
set cbtics (0,2*pi, 4*pi)

set pm3d depthorder
set view map
splot 'sine_p.dat' u 1:2:3 w pm3d notitle


set term epslatex color solid size 11cm,5cm
set output 'sine_imag.eps'

set xlabel '$x$'
set ylabel '$y$'

set xrange [-3*pi:3*pi]
set yrange [0:3*pi]
set zrange [-5:5]
set cbrange [-5:5]
set cbtics (-5,0,5)

set pm3d depthorder
set view map
splot 'sine_p.dat' u 1:2:4 w pm3d notitle


reset
set term epslatex color solid size 11cm,5cm header "\\scriptsize"
set output 'sine_diff.eps'

set xlabel '$\theta$' offset 0,0
set ylabel '$\phi_R(\pi/2+\Delta,\theta)-\phi_R(\pi/2-\Delta,\theta)$' offset 2,0

a=0.5
set xrange [0+a:pi-a]
set yrange [-2:16]

set ytics (0,2*pi,4*pi)

set key bottom right

plot '< paste lower0.01.dat higher0.01.dat' u 2:($3-$9) w l title '$\Delta=0.01$',\
     '< paste lower0.005.dat higher0.005.dat' u 2:($3-$9) w l title '$\Delta=0.005$',\
     '< paste lower0.001.dat higher0.001.dat' u 2:($3-$9) w l title '$\Delta=0.001$'



reset
set term epslatex color solid size 5cm,5cm
set output 'sine_quadrant_real.eps'
set view 60,315

set xlabel '$x$' offset 1,-1
set ylabel '$y$' offset -1,-1
set zlabel '$\Re\ 4\mathrm{atan}(e^{x+iy})$' offset -2,0 rotate by 90

set zrange [2:10]
set cbrange [2:10]

set xtics (0,3,6,9)
set ytics (0,3,6,9)
set ztics (3,6,9,14)
set cbtics (3,6,9,14)

splot 'i.dat' u 1:2:3 w pm3d notitle


set output 'sine_quadrant_imag.eps'
set view 60,315

set xlabel '$x$' offset 1,-1
set ylabel '$y$' offset -1,-1
set zlabel '$\Im\ 4\mathrm{atan}(e^{x+iy})$' offset -2,0 rotate by 90

set zrange [-8:8]
set cbrange [-8:8]

set xtics (0,3,6,9)
set ytics (0,3,6,9)
set ztics (-8,0,8)
set cbtics (-8,0,8)

splot 'i.dat' u 1:2:5 w pm3d notitle