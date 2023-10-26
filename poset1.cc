#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <set>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::exit;
using std::list;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

using poset_elem = string;
using vectorOfStrings = vector<poset_elem>;
using posetRelationsArray = vector<vector<int>>;
using identificator = unsigned long;
using poset_t = pair<vectorOfStrings *, posetRelationsArray *>;

unordered_map<identificator, poset_t *> allPosets;

void printVectorOfStrings(vectorOfStrings const &vec)
{
  size_t n = vec.size();

  cout << "vecOfStrings: ";
  for (size_t i = 0; i < n; i++)
    cout << vec[i] << ", ";
  cout << "\n";
}

void printArrOfRelations(posetRelationsArray const &arr)
{
  size_t rows = arr.size();
  size_t columns = arr[0].size();

  cout << "Array of relations: \n";
  cout << " ";

  for(size_t i = 0; i < columns; i++)
    cout << i << " ";

  cout << "\n";

  for(size_t i = 0; i < rows; i++)
  {
    cout << i << " ";

    for(size_t j = 0; j < columns; j++)
      cout << arr[i][j] << " ";
    
    cout << "\n";
  }

}

// tylko zeby bylo teraz, przypisuje id zawsze o 1 wiekszy od najwyzszego id.
unsigned long poset_new(void)
{
  unsigned long id = 0;

  if (!allPosets.empty())
  {
    size_t sizeAllPosets = allPosets.size();

    if (++sizeAllPosets == 0)
    {
      cerr << "Poset_new - no free IDs to use\n";
      exit(-1);
    }
    for (const auto &entry : allPosets)
    {
      if (entry.first >= id)
      {
        id = entry.first;
        id++;
      }
    }
  }

  poset_t *newPoset = new poset_t;
  vectorOfStrings *newVecOfPosetElem = new vectorOfStrings;
  posetRelationsArray *newP = new posetRelationsArray;

  newPoset->first = newVecOfPosetElem;
  newPoset->second = newP;

  allPosets.insert({id, newPoset});

  return id;
}

void poset_delete(unsigned long id)
{
  auto it = allPosets.find(id);
  if (it != allPosets.end())
  {
    allPosets.erase(it);
  }
}

size_t poset_size(unsigned long id)
{
  auto it = allPosets.find(id);

  if (it != allPosets.end())
  {
    // poset_t = pair<vectorOfStrings*, posetRelationsArray*>;
    // it->first = id, it->second = pair
    vectorOfStrings *v = it->second->first;
    return v->size();
  }

  return 0;
}

bool poset_insert(unsigned long id, char const *value)
{
  auto it = allPosets.find(id);
  if (it != allPosets.end())
  {
    vectorOfStrings *v = it->second->first;
    for (const poset_elem &str : *v)
    {
      if (str == value)
      {
        return false;
      }
    }
    v->push_back(value);

    posetRelationsArray *p = it->second->second;
    p->push_back(vector<int>(p->size(), -1)); // dodanie nowego wiersza wypelnionego -1

    // dodanie nowej kolumny
    for (vector<int> &row : *p)
    {
      row.push_back(-1);
    }

    return true;
  }

  return false;
}

bool poset_remove(unsigned long id, char const *value);

bool poset_add(unsigned long id, char const *value1, char const *value2);

bool poset_del(unsigned long id, char const *value1, char const *value2);

bool poset_test(unsigned long id, char const *value1, char const *value2);

void poset_clear(unsigned long id);

void Test_insert()
{
  vectorOfStrings *v = allPosets[0]->first;
  posetRelationsArray *p = allPosets[0]->second;

  for (const poset_elem &str : *v)
  {
    cout << str << " ";
  }
  cout << std::endl;

  for (vector<int> &row : *p)
  {
    for (int &num : row)
    {
      cout << num << " ";
    }
    cout << std::endl;
  }
}

int main()
{
  int id = poset_new();
  int id2 = poset_new();
  int id3 = poset_new();
  poset_delete(id3);
  int id4 = poset_new();
  cout << id << id2 << id4;
  bool t1 = poset_insert(id, "A");
  bool t2 = poset_insert(id, "B");
  bool t3 = poset_insert(id2, "B");
  cout << t1 << t2 << t3 << std::endl;
  Test_insert();
  int s = poset_size(id);
  cout << s << std::endl;

  return 0;
}