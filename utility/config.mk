CXX = g++
DEPS_BIN = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g
LDLIB = 
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p

PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/


SOURCES = src/main.cpp

HEADERS = 

BIN = bin/main


#bin/...: ...
bin/main: build/src/main.o

LIB = 

#lib/...: ...
