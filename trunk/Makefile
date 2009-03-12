#
# Makefile for CPP
#

# Compiler and compiler options:
CC	  = g++
CXX       = g++ 
CXXFLAGS  = -pipe -O3 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
#CXXFLAGS += -lcurses
#CXXFLAGS += -g
#CXXFLAGS += -pg
#CXXFLAGS += -floop-optimize2



PROG = lennart

#what to do with all

.PHONY: all
all: $(PROG)

.PHONY: debug
debug: main.o board.o game.o gui.o player.o evaluation.o timer.o transposition.o functions.o move.o
	$(CXX) -ggdb -pg -o $@ $^

.PHONY: clean
clean:
	rm *.o -f

# Linking:
lennart: main.o board.o game.o gui.o player.o evaluation.o timer.o transposition.o functions.o move.o
	$(CXX) -o $@ $^

# Dependencies, the implicit rule .cc => .o is used
timer.o: timer.h timer.cc
board.o: board.h board.cc
player.o: player.h player.cc
main.o: main.cc
evaluation.o: evaluation.h evaluation.cc
game.o: game.h game.cc
gui.o: gui.h gui.cc
transposition.o: transposition.h transposition.cc
functions.o: functions.h functions.cc
move.o: move.h move.cc
