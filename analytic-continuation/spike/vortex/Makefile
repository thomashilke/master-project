all: a_and_f_funcs_vortex.pdf
	cp a_and_f_funcs_vortex.pdf a_and_f_funcs_vortex.tex ../../Log/figures/

a_and_f_funcs_vortex.pdf: a_and_f_funcs_vortex.eps
	epstopdf a_and_f_funcs_vortex.eps

a_and_f_funcs_vortex.eps: graph.plot func_a.m func_f.m
	gnuplot graph.plot


clean:
	rm -rf *~ *.pdf *.tex *.eps