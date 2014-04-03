# Makefile
# Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu
CC=g++

CPPFLAGS=-std=c++0x -pedantic -Wall -Wextra -O -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -g
CPPFLAGS+=$(shell pkg-config --cflags gtkmm-3.0)

LDFLAGS=
LDFLAGS+=$(shell pkg-config --libs gtkmm-3.0)

all: main 

test: main
	./main

main: main.o fractal.o mandelbrot.o
	g++ $^ -o $@ $(LDFLAGS)	

fractal.o: fractal.cc fractal.h mandelbrot.h

mandelbrot.o: mandelbrot.cc mandelbrot.h fractal.h

clean:
	rm -rf *.o main
