CXX = g++
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c17

all: poset poset_test1 poset_test2

poset.o: poset.cc poset.h
	$(CXX) $(CFLAGS) -c poset.cc

poset: poset.o
	$(CXX) $(CFLAGS) poset.o -o poset


#do testow
poset1.o: poset.cc
	$(CXX) $(CFLAGS) -c poset.cc

poset_test1.o: poset_test1.c
	$(CC) $(CFLAGS) -c poset_test1.c

poset_test2.o: poset_test2.cc
	$(CXX) $(CFLAGS) -c poset_test2.c

poset_test1: poset_test1.o poset1.o
	$(CXX)  poset_test1.o poset.o -o poset_test1

poset_test2: poset_test2.o poset1.o
	$(CXX)  poset_test2.o poset1.o -o poset_test2




clean:
	rm -f *.o poset poset_test1 poset_test2


