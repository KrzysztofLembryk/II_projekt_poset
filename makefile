CFLAGS = -Wall -Wextra -O2 -std=c++17

all: poset1

poset1.o: poset1.cc poset1.h
	g++ $(CFLAGS) -c poset1.cc

poset1: poset1.o
	g++ $(CFLAGS) poset1.o -o poset1

clean:  
	rm -f *.o poset1 