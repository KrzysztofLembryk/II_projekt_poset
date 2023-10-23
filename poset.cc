#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <set>

#ifdef NDEBUG
  bool constexpr debug = false;
#else
  bool constexpr debug = true;
#endif

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::list;
using std::set;
using std::exit;

using poset_t = std::unordered_map<string, list<string>*>;

std::unordered_map<unsigned long, poset_t*> allPosets;

unsigned long poset_new(void)
{
  static set<unsigned long> allPosetIDs;
  static unsigned long id = 0;
  bool inserted = false;
  int loopCounter = 0;

  while(true)
  {
    auto search = allPosetIDs.find(id);

    if (search != allPosetIDs.end())
      {
        id++;

        if(id == 0)
        {
          loopCounter++;

          if(loopCounter >= 2)
          {
            cerr << "Poset_new - no free IDs to use\n";
            exit(-1);
          }
        }
      }
    else
    {
      poset_t *newPoset = new poset_t;

      allPosetIDs.insert(id);
      allPosets.insert({id, newPoset});

      return id;
    }
  }
}

void TEST_poset_new()
{
  int id1 = poset_new();
  int id2 = poset_new();

  if(allPosets.empty())
    cout << "All posets EMPTY\n";
  else
    cout << "succes\n";

  list<string> *list1 = new list<string>;

  allPosets.at(id1)->insert({"trol", list1});
  allPosets.at(id1)->at("trol")->push_front("WOLWOLWOOW");
  allPosets.at(id1)->at("trol")->push_front("xdxdxdxd");

  for(auto iter = allPosets.at(id1)->begin(); iter != allPosets.at(id1)->end(); ++iter)
  {
    // iter->first = key (id), iter->second = value (poset)
    cout << "id in poset: " << iter->first << "\n";
    auto lst = iter->second;
    cout << "values: ";
    for(auto iter2 = lst->begin(); iter2 != lst->end(); iter2++)
    {
       cout << *iter2 << ", ";
    }
  }
    

}

int main()
{
  TEST_poset_new();
  return 0;
}
