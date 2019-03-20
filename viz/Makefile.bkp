TARGETS= viz mktestfile

all: $(TARGETS)

viz: viz.cpp dampeddynamic.hpp
	g++ -o $@ $<  -lGL -lX11 -ISmartPaper SmartPaper/renderer/*.cpp -g

mktestfile: mktestfile.cpp
	g++ -o $@ $<

clean:
	rm -fr $(TARGETS) *~