#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <set>
#include <vector>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

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

using index = size_t;
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
  cout << "\n\n";
}

void printArrOfRelations(posetRelationsArray const &arr)
{
  size_t rows = arr.size();
  size_t columns = arr[0].size();

  cout << "Array of relations: \n";
  cout << "  ";

  for (size_t i = 0; i < columns; i++)
    cout << i << " ";

  cout << "\n";

  for (size_t i = 0; i < rows; i++)
  {
    cout << i << " ";

    for (size_t j = 0; j < columns; j++)
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
  posetRelationsArray *newRelationsArr = new posetRelationsArray;

  newPoset->first = newVecOfPosetElem;
  newPoset->second = newRelationsArr;

  allPosets.insert({id, newPoset});

  return id;
}

void poset_delete(unsigned long id)
{
  auto it = allPosets.find(id);
  if (it != allPosets.end())
  {
    delete it->second->first;
    delete it->second->second;
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
        return false;
    }

    poset_elem elemToAdd(value);
    v->push_back(elemToAdd);

    posetRelationsArray *p = it->second->second;

    // dodanie nowego wiersza wypelnionego -1
    // -1 oznacza brak relacji
    p->push_back(vector<int>(p->size(), -1));

    // dodanie nowej kolumny
    for (vector<int> &row : *p)
      row.push_back(-1);

    return true;
  }

  return false;
}

bool poset_remove(unsigned long id, char const *value)
{
  auto iter = allPosets.find(id);
  if (iter != allPosets.end())
  {
  }
  return false;
}

void findIndexesOfGivenValues(long long &index1, long long &index2,
                  char const *value1, char const *value2, vectorOfStrings *v)
{
  index1 = -1;
  index2 = -1;
  size_t vSize = v->size();

  for (index i = 0; i < vSize; i++)
  {
    if (v->at(i) == value1)
      index1 = i;
    if (v->at(i) == value2)
      index2 = i;
    if (index1 != -1 && index2 != -1)
      break;
  }
}

bool poset_add(unsigned long id, char const *value1, char const *value2)
{
  auto it = allPosets.find(id);

  if (it != allPosets.end())
  {
    long long index1;
    long long index2;
    vectorOfStrings *v = it->second->first;

    findIndexesOfGivenValues(index1, index2, value1, value2, v);
  
    // there is no element (value1 or value2) in a set
    if (index1 == -1 || index2 == -1)
      return false;
    else
    {
      posetRelationsArray *relationArr = it->second->second;

      // if there is an edge between value1 and value2
      if (relationArr->at(index1)[index2] == 1 || relationArr->at(index2)[index1] == 1)
        return false;
      else
      {
        relationArr->at(index1)[index2] = 1; // edge from index1(value1) to index2 (value2)

        // now add edges that will result from transitivity
        size_t arrSize = relationArr->size();

        for (index i = 0; i < arrSize; i++)
        {
          if (relationArr->at(i)[index1] != -1 && relationArr->at(i)[index2] == -1)
            relationArr->at(i)[index2] = 2;
        }

        return true;
      }
    }
  }

  return false;
};

bool poset_del(unsigned long id, char const *value1, char const *value2);

bool poset_test(unsigned long id, char const *value1, char const *value2)
{
  auto it = allPosets.find(id);
  if (it != allPosets.end())
  {
    long long index1; 
    long long index2;
    vectorOfStrings *v = it->second->first;

    findIndexesOfGivenValues(index1, index2, value1, value2, v);

    // there is no element (value1 or value2) in a set
    if (index1 == -1 || index2 == -1) 
      return false;
    else
    {
      posetRelationsArray *p = it->second->second;

      // if there is an edge between value1 and value2 (value1 < value2)
      if (p->at(index1)[index2] == 1) 
        return true;
    }
  }

  return false;
}

void poset_clear(unsigned long id)
{
  auto it = allPosets.find(id);
  if (it != allPosets.end())
  {
    vectorOfStrings *v = it->second->first;
    posetRelationsArray *p = it->second->second;
    v->clear();
    p->clear();
  }
}

// ------------- TESTS -------------

void TEST_poset_new_delete_insert_add()
{
  cout << "----- TEST_poset_new_delete_insert -----\n";
  cout << "NEW/DELETE SECTION TEST: \n";

  unsigned long id1 = poset_new();
  unsigned long id2 = poset_new();
  unsigned long id3 = poset_new();

  assert(allPosets.size() == 3 && "allPosets.size is not equal 3, creating posets went wrong\n");
  poset_delete(id1);
  assert(allPosets.size() == 2 && "allPosets.size is not equal 2, deleting went wrong\n");

  unsigned long id4 = poset_new();

  assert(id2 == 1 && "id2 should equal 1\n");

  cout << "INSERT SECTION TEST: \n";

  assert(poset_insert(id1, "xd") == false && "poset id1 does not exist, but inserting was a success - WRONG\n");
  assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id3, "B") == true && "insert into id3 poset was not succesful\n");

  cout << "ADD SECTION TEST: \n";

  assert(poset_add(id2, "A", "B") == false);
  assert(poset_add(id2, "A", "X") == true);

  cout << "TESTS PASSED\n\n";
  cout << "printing id2=" << id2 << " poset: \n\n";

  printVectorOfStrings(*allPosets[id2]->first);
  printArrOfRelations(*allPosets[id2]->second);
}

void test()
{
  poset_t *newPoset = new poset_t;
  vectorOfStrings newVecOfPosetElem;
  posetRelationsArray newP;

  newPoset->first = &newVecOfPosetElem;
  newPoset->second = &newP;

  allPosets.insert({0, newPoset});

  allPosets[0]->first->push_back("69");
  allPosets[0]->first->push_back("Alaaa");
  allPosets[0]->second->push_back(vector<int>());
  allPosets[0]->second->push_back(vector<int>());
  posetRelationsArray *arr = allPosets[0]->second;
  (*arr)[0].push_back(1);
  (*arr)[0].push_back(0);
  (*arr)[1].push_back(2);
  (*arr)[1].push_back(3);

  printVectorOfStrings(*(allPosets[0]->first));
  printArrOfRelations(*arr);
}

int main()
{

  TEST_poset_new_delete_insert_add();

  return 0;
}