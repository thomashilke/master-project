all: f_and_h_funcs_monopole.pdf
	cp f_and_h_funcs_monopole.pdf f_and_h_funcs_monopole.tex ../../../Log/figures/

f_and_h_funcs_monopole.pdf: f_and_h_funcs_monopole.eps
	epstopdf f_and_h_funcs_monopole.eps

f_and_h_funcs_monopole.eps: graph.plot func_f.m func_h.m
	gnuplot graph.plot


clean:
	rm -rf *~ *.pdf *.eps *.tex