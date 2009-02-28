#
# Makefile for CPP
#

# Compiler and compiler options:
CC	  = g++
CXX       = g++ 
CXXFLAGS  = -pipe -O3 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
#CXXFLAGS += -g -pg
#CXXFLAGS += -floop-optimize2



PROG = lennart timertest aitest

#what to do with all

.PHONY: all
all: $(PROG)

.PHONY: debug
debug: main.o board.o game.o gui.o search.o evaluation.o timer.o functions.o transposition.o
	$(CXX) -ggdb -pg -o $@ $^

.PHONY: clean
clean:
	rm *.o -f

# Linking:
lennart: main.o board.o game.o gui.o search.o evaluation.o timer.o functions.o transposition.o
	$(CXX) -o $@ $^

timertest: timertest.o timer.o

aitest: aitest.o board.o game.o gui.o search.o evaluation.o timer.o functions.o transposition.o

# Dependencies, the implicit rule .cc => .o is used
timertest.o: timertest.cc timer.h
timer.o: timer.h timer.cc
board.o: board.h board.cc
search.o: search.h search.cc
main.o: main.cc
evaluation.o: evaluation.h evaluation.cc
game.o: game.h game.cc
gui.o: gui.h gui.cc
aitest.o: aitest.cc
functions.o: functions.h functions.cc
transposition.o: transposition.h transposition.cc
