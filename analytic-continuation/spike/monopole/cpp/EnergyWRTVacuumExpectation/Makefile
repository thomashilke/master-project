all: varying_vacuum_f.pdf varying_vacuum_h.pdf varying_vacuum_energy.pdf
	cp varying_vacuum_f.pdf varying_vacuum_h.pdf varying_vacuum_energy.pdf varying_vacuum_f.tex varying_vacuum_h.tex varying_vacuum_energy.tex ../../../../Log/figures/

varying_vacuum_f.pdf: varying_vacuum_f.eps
	epstopdf $<

varying_vacuum_h.pdf: varying_vacuum_h.eps
	epstopdf $<

varying_vacuum_energy.pdf: varying_vacuum_energy.eps
	epstopdf $<

varying_vacuum_f.eps: graph.plot
	gnuplot $<

varying_vacuum_h.eps: graph.plot
	gnuplot $<

varying_vacuum_energy.eps: graph.plot
	gnuplot $<

clean:
	rm -rf *~ *.pdf *.eps *.tex