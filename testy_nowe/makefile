all:
	g++ -ggdb3 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -DNDEBUG -Wall -Wextra -O2 -std=c++17 -c poset.cc -o poset.o
	g++ -Wall -Wextra -O2 -std=c++17 -c testy.cpp -o poset_testy.o
	g++ poset.o poset_testy.o -o poset_testy
