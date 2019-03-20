all: varying_lambda_f.pdf varying_lambda_h.pdf varying_lambda_energy.pdf
	cp varying_lambda_f.pdf varying_lambda_h.pdf varying_lambda_energy.pdf varying_lambda_f.tex varying_lambda_h.tex varying_lambda_energy.tex ../../../../Log/figures/

varying_lambda_f.pdf: varying_lambda_f.eps
	epstopdf $<

varying_lambda_h.pdf: varying_lambda_h.eps
	epstopdf $<

varying_lambda_energy.pdf: varying_lambda_energy.eps
	epstopdf $<

varying_lambda_f.eps: graph.plot
	gnuplot $<

varying_lambda_h.eps: graph.plot
	gnuplot $<

varying_lambda_energy.eps: graph.plot
	gnuplot $<

clean:
	rm -rf *~ *.pdf *.eps *.tex