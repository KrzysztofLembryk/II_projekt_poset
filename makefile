CXX = g++
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c17
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

all: poset poset_test1 poset_test2

poset.o: poset.cc poset.h
	$(CXX) $(CXXFLAGS) -c poset.cc

poset: poset.o
	$(CXX) $(CXXFLAGS) poset.o -o poset


#do testow
poset1.o: poset.cc
	$(CXX) $(CXXFLAGS) -c poset.cc -o poset1.o

poset_test1.o: poset_test1.c
	$(CC) $(CFLAGS) -c poset_test1.c

poset_test2.o: poset_test2.cc
	$(CXX) $(CXXFLAGS) -c poset_test2.cc

poset_test1: poset_test1.o poset1.o
	$(CXX)  poset_test1.o poset1.o -o poset_test1

poset_test2: poset_test2.o poset1.o
	$(CXX)  poset_test2.o poset1.o -o poset_test2

clean:
	rm -f *.o poset poset_test1 poset_test2