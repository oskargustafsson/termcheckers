#
# Makefile for CPP
#

# Compiler and compiler options:
CC	  = g++
CXX       = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
CXXFLAGS += -g


PROG = lennart timertest

#what to do with all

.PHONY: all
all: $(PROG)

.PHONY: debug
debugg: main.o board.o game.o gui.o search.o evaluation.o
	$(CXX) -ggdb -o $@ $^

# Linking:
lennart: main.o board.o game.o gui.o search.o evaluation.o
	$(CXX) -o $@ $^

timertest: timertest.o timer.o

# Dependencies, the implicit rule .cc => .o is used
timertest.o: timertest.cc timer.h
timer.o: timer.cc timer.h
board.o: board.cc board.h
search.o: search.cc search.h
main.o: main.cc
evaluation.o: evaluation.cc evaluation.h
game.o: game.cc game.h
gui.o: gui.cc gui.h
