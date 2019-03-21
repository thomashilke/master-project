CXX = g++
DEPS_BIN = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g
LDLIB = -lGL -lX11
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p

PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/


SOURCES = src/viz.cpp \
	src/mktestfile.cpp \
	src/SmartPaper/renderer/renderer.cpp \
	src/SmartPaper/renderer/texture.cpp \
	src/SmartPaper/renderer/x11utilities.cpp

HEADERS = 

BIN = bin/viz bin/mktestfile


#bin/...: ...
bin/viz: build/src/viz.o build/src/SmartPaper/renderer/renderer.o build/src/SmartPaper/renderer/texture.o build/src/SmartPaper/renderer/x11utilities.o
bin/mktestfile: build/src/mktestfile.o

LIB = 

#lib/...: ...
