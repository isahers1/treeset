#
# Makefile for CS 70 treeset Assignment
#

all: treeset-test

treeset-test: treeset-test.o
	clang++ -o treeset-test treeset-test.o -L/usr/lib/cs70lib -l testinglogger -l randuint32

treeset-test.o: treeset-test.cpp treeset.hpp treeset-private.hpp
	clang++ -c -g -std=c++17 -Wall -Wextra -pedantic treeset-test.cpp

clean:
	rm -rf treeset-test *.o *.dSYM
