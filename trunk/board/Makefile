#
# Makefile for CPP
#

# Compiler and compiler options:
CXX       = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast

# Linking:
btest: btest.o board.o
	$(CXX) -o $@ $^

# Dependencies, the implicit rule .cc => .o is used
board.o: board.cc board.h
btest.o: btest.cc