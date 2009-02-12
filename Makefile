#
# Makefile for CPP
#

# Compiler and compiler options:
CXX       = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast


# Linking:
checkers: checkers.o board.o ui.o game.o searchtree.o evaluation.o
	$(CXX) -o $@ $^

# Dependencies, the implicit rule .cc => .o is used
board.o: board.cc board.h
checkers.o: checkers.cc checkers.h
searchtree.o: searchtree.cc searchtree.h
evaluation.o: evaluation.cc evaluation.h
game.o: game.cc game.h
