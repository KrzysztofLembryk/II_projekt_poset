#include <iostream>
#include <string>
#include <unordered_map>
#include <list>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::list;

using poset_t = std::unordered_map<string, list<string>>;

std::unordered_map<int, poset_t> allPosets;

#ifdef NDEBUG
  bool constexpr debug = false;
#else
  bool constexpr debug = true;
#endif
