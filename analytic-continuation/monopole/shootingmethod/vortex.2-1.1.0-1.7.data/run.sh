#!/bin/bash

n=60
r=20
xi=1.

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex0.1.dat outp=vortexp0.1.dat chi=0.1 inpname=vortexp.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex0.3.dat outp=vortexp0.3.dat chi=0.3 inpname=vortexp0.1.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex0.5.dat outp=vortexp0.5.dat chi=0.5 inpname=vortexp0.3.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex0.7.dat outp=vortexp0.7.dat chi=0.7 inpname=vortexp0.5.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex0.9.dat outp=vortexp0.9.dat chi=0.9 inpname=vortexp0.7.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex1.1.dat outp=vortexp1.1.dat chi=1.1 inpname=vortexp0.9.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex1.3.dat outp=vortexp1.3.dat chi=1.3 inpname=vortexp1.1.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex1.5.dat outp=vortexp1.5.dat chi=1.5 inpname=vortexp1.3.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex1.7.dat outp=vortexp1.7.dat chi=1.7 inpname=vortexp1.5.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex1.9.dat outp=vortexp1.9.dat chi=1.9 inpname=vortexp1.7.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex2.1.dat outp=vortexp2.1.dat chi=2.1 inpname=vortexp1.9.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex2.3.dat outp=vortexp2.3.dat chi=2.3 inpname=vortexp2.1.dat

../shootverbin2-1 threads=8 r=$r n=$n xi=$xi inpsource=file \
    outf=vortex2.5.dat outp=vortexp2.5.dat chi=2.5 inpname=vortexp2.3.dat