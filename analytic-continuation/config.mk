include site-config.mk

CXX = g++
DEPS_BIN = g++
DEPSFLAGS = -I../utility/src/ -I../viz/src/ -I$(LAPACKE_INCLUDE_DIR)
CXXFLAGS = -g -std=c++11 -I../utility/src/ -I../viz/src/ -I$(LAPACKE_INCLUDE_DIR)
LDFLAGS = -g -L$(LAPACKE_LIB_DIR)
LDLIB =  -llapacke
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p

PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/


SOURCES =\
        src/composite/main.cpp \
	src/gravity/classicalgravity.cpp \
	src/gravity/minimalcoupling.cpp \
	src/gravity/testscheme.cpp \
	src/log/convergence.cpp \
	src/log/main.cpp \
	src/monopole/bps.cpp \
	src/monopole/classicalmonopole.cpp \
	src/monopole/main.cpp \
	src/monopole/shootingmethod/main.cpp \
	src/monopole/shootingmethod/ordercheck.cpp \
	src/monopole/shootingmethod/shootingverbin2-1.cpp \
	src/monopole/shootingmethod/shootingverbin3-1.cpp \
	src/sinegordon/branchcuts.cpp \
	src/sinegordon/main.cpp \
	src/sinegordon/quadrants.cpp \
	src/sinegordon/singularity.cpp \
	src/sqrt/convergence.cpp \
	src/sqrt/main.cpp \
	src/testcoord.cpp \
	src/testmatrix.cpp \
	src/testrange.cpp \
	src/vortex/classicalvortex.cpp \
	src/vortex/main.cpp \
	src/vortex/testinterpfunc.cpp

HEADERS = 

BIN = bin/composite \
      bin/gravity-cg bin/gravity-mc \
      bin/log \
      bin/monopole \
      bin/monopole-clas \
      bin/monopole-bps \
      bin/sinegordon \
      bin/sqrt \
      bin/vortex

bin/composite: build/src/composite/main.o
bin/gravity-cg: build/src/gravity/classicalgravity.o
bin/gravity-mc: build/src/gravity/minimalcoupling.o
bin/log: build/src/log/main.o
bin/monopole: build/src/monopole/main.o
bin/monopole-clas: build/src/monopole/classicalmonopole.o
bin/monopole-bps: build/src/monopole/bps.o
bin/monopole-sm-ve2: build/src/monopole/shootingmethod/shootingverbin2-1.o
bin/monopole-sm-ve3: build/src/monopole/shootingmethod/shootingverbin3-1.o
bin/sinegordon: build/src/sinegordon/main.o
bin/sqrt: build/src/sqrt/main.o
bin/vortex: build/src/vortex/main.o

LIB = 

#lib/...: ...
