#
# Makefile for CPP
#

# Compiler and compiler options:
CXX       = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
CXXFLAGS += -g


# Linking:
lennart: main.o board.o game.o gui.o search.o evaluation.o
	$(CXX) -o $@ $^

# Dependencies, the implicit rule .cc => .o is used
board.o: board.cc board.h
search.o: search.cc search.h
main.o: main.cc
evaluation.o: evaluation.cc evaluation.h
game.o: game.cc game.h
gui.o: gui.cc gui.h
