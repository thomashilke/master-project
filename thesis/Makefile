PDFLATEX = pdflatex

STDOUT_REDIRECT = /dev/null
STDOUT_REDIRECT = &1

.DEFAULT_GOAL = all
.PHONY = all pdf clean

SOURCES = src/document.tex

all: pdf

pdf: pdf/document.pdf

pdf/document.pdf:  build/document.pdf
	@echo "[CP]" $@
	@cp $< $@

build/document.pdf: $(SOURCES)
	@echo "[PDFLATEX]" $@
	@cd $(dir $<) && $(PDFLATEX) -output-directory=../$(dir $@) -halt-on-error -jobname=document $(patsubst src/%.tex,%.tex,$<) >$(STDOUT_REDIRECT)
	@cd $(dir $<) && $(PDFLATEX) -output-directory=../$(dir $@) -halt-on-error -jobname=document $(patsubst src/%.tex,%.tex,$<) >$(STDOUT_REDIRECT)
	@cd $(dir $<) && $(PDFLATEX) -output-directory=../$(dir $@) -halt-on-error -jobname=document $(patsubst src/%.tex,%.tex,$<) >$(STDOUT_REDIRECT)

biblatex:
	@echo "[BIBLATEX] document.bcf"
	cd build; biber document > biblatex-target.log

clean:
	@rm -rf build/*
	@find . -name "*~" -delete


