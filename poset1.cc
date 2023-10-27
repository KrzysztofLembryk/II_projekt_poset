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

const int RELATION = 1;
const int NO_RELATION = -1;
const int RELATION_TRANSITIVITY = 2;
const int RELATION_IM_LARGER = 3;

namespace {

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
  cout << "   ";

  for (size_t i = 0; i < columns; i++)
    cout << i << "  ";

  cout << "\n";

  for (size_t i = 0; i < rows; i++)
  {
    cout << i << " ";

    for (size_t j = 0; j < columns; j++)
    {
      if (arr[i][j] != -1)
        cout << " ";
      cout << arr[i][j] << " ";
    }

    cout << "\n";
  }
}

void printPoset(poset_t const *p)
{
  printVectorOfStrings(*(p->first));
  printArrOfRelations(*(p->second));
}

void deleteRelationsTransitivity(posetRelationsArray *relArr, 
size_t currentElem, size_t idxOfElemToDelete, size_t nbrOfRows)
{
  if (relArr->at(currentElem)[idxOfElemToDelete] == RELATION ||
          relArr->at(currentElem)[idxOfElemToDelete] == RELATION_TRANSITIVITY)
        {
          for(size_t j = 0; j < nbrOfRows; j++)
          {
            if((relArr->at(idxOfElemToDelete)[j] == RELATION ||
            relArr->at(idxOfElemToDelete)[j] == RELATION_TRANSITIVITY) && 
            relArr->at(currentElem)[j] == RELATION_TRANSITIVITY)
            {
              relArr->at(currentElem)[j] = NO_RELATION;
              relArr->at(j)[currentElem] = NO_RELATION;
            }
          }
        }
}

void deleteRelationsLarger(posetRelationsArray *relArr, 
size_t currentElem, size_t idxOfElemToDelete, size_t nbrOfRows)
{
  if (relArr->at(currentElem)[idxOfElemToDelete] == RELATION_IM_LARGER)
  {
    for(size_t j = 0; j < nbrOfRows; j++)
    {
      if(relArr->at(idxOfElemToDelete)[j] == RELATION_IM_LARGER &&
      relArr->at(j)[currentElem] == RELATION_TRANSITIVITY)
      {
        relArr->at(j)[currentElem] = NO_RELATION;
        relArr->at(currentElem)[j] = NO_RELATION;
      }
    }
  }
}

void checkIfElemExistInVecOfStr(vectorOfStrings *v, char const *value, 
size_t &idx, bool &exist)
{
  size_t vSize = v->size();
  for (size_t i = 0; i < vSize; i++)
    {
      if ((*v)[i] == value)
      {
        exist = true;
        idx = i;
        break;
      }
    }
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


bool somethingIsBetweenTwoElem(posetRelationsArray *relationArr, 
long long const idx1, long long const idx2)
{
  size_t nbrOfRows = relationArr->size();
  
  for(size_t i = 0; i < nbrOfRows; i++)
  {
    if((relationArr->at(idx1)[i] == RELATION || 
    relationArr->at(idx1)[i] == RELATION_TRANSITIVITY) && 
    (relationArr->at(i)[idx2] == RELATION ||
    relationArr->at(i)[idx2] == RELATION_TRANSITIVITY))
    {
      return true;
    }
  }

  return false;
}

bool isAnythingOnTheRight(posetRelationsArray *relArr, long long const idx2)
{
  size_t nbrOfCol = relArr->at(idx2).size();

  for(size_t i = 0; i < nbrOfCol; i++)
  {
    if(relArr->at(idx2)[i] == RELATION ||
    relArr->at(idx2)[i] == RELATION_TRANSITIVITY)
    {
      return true;
    }
  }

  return false;
}

bool isAnythingOnTheLeft(posetRelationsArray *relArr, long long const idx1)
{
  size_t nbrOfCol = relArr->at(idx1).size();

  for(size_t i = 0; i < nbrOfCol; i++)
  {
    if(relArr->at(idx1)[i] == RELATION_IM_LARGER)
    {
      return true;
    }
  }
  return false;
}

bool relationGoodToDelete(posetRelationsArray *relationArr, 
long long const idx1, long long const idx2, 
bool &isOnTheLeft, bool &isOnTheRight)
{
  if(!somethingIsBetweenTwoElem(relationArr, idx1, idx2))
  {
    isOnTheLeft = isAnythingOnTheLeft(relationArr, idx1);
    isOnTheRight = isAnythingOnTheRight(relationArr, idx2);
          
    // jesli z obu stron cos mamy, to nie mozemy usunac tej relacji
    // bo zostawimy dziure w ciagu nierownosci
    if(isOnTheLeft && isOnTheRight)
      return false;
     
    return true;
  }
  
  return false;
}

void poset_del_IsOnTheLeft(posetRelationsArray *relArr, 
long long const idx1, long long const idx2)
{
  size_t nbrOfRows = relArr->at(idx2).size();

  for(size_t i = 0; i < nbrOfRows; i++)
  {
    if(relArr->at(i)[idx1] == RELATION || 
    relArr->at(i)[idx1] == RELATION_TRANSITIVITY)
    {
      if(relArr->at(i)[idx2] == RELATION_TRANSITIVITY)
      {
        relArr->at(i)[idx2] = NO_RELATION;
        relArr->at(idx2)[i] = NO_RELATION;
      }
    }
  }

}

void poset_del_IsOnTheRight(posetRelationsArray *relArr, 
long long const idx1, long long const idx2)
{
  size_t nbrOfRows = relArr->at(idx2).size();

  for(size_t i = 0; i < nbrOfRows; i++)
  {
    if(relArr->at(i)[idx2] == RELATION_IM_LARGER)
    {
      if(relArr->at(idx1)[i] == RELATION_TRANSITIVITY)
      {
        relArr->at(i)[idx1] = NO_RELATION;
        relArr->at(idx1)[i] = NO_RELATION;
      }
    }
  }

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

//DONE
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

//DONE
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
  if(value == nullptr)
    return false;
  
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

    //element is in relation with itself
    p->at(p->size()-1)[p->size()-1] = 1;


    return true;
  }

  return false;
}

bool poset_remove(unsigned long id, char const *value)
{
  if(value == nullptr)
    return false;
  
  bool elemExists = false;
  size_t idxOfElem;
  auto iter = allPosets.find(id);

  if (iter != allPosets.end())
  {
    vectorOfStrings *v = iter->second->first;

    checkIfElemExistInVecOfStr(v, value, idxOfElem, elemExists);  

    if (elemExists)
    {
      (*v).erase((*v).begin() + idxOfElem);

      posetRelationsArray *relationArr = iter->second->second;
      size_t nbrOfRows = relationArr->size();
      vector<int> *rowVec;

      for (size_t i = 0; i < nbrOfRows; i++)
      {
        if(i != idxOfElem)
        {
          deleteRelationsTransitivity(relationArr, i, idxOfElem, nbrOfRows);
          deleteRelationsLarger(relationArr, i, idxOfElem, nbrOfRows);
        
          rowVec = &(relationArr->at(i));
          rowVec->erase(rowVec->begin() + idxOfElem);  
        }
      }

      relationArr->erase(relationArr->begin() + idxOfElem);
      
      return true;
    }
  }
  return false;
}

bool poset_add(unsigned long id, char const *value1, char const *value2)
{
  if(value1 == nullptr || value2 == nullptr)
    return false;
  
  auto it = allPosets.find(id);

  if (it != allPosets.end())
  {
    long long index1;
    long long index2;
    vectorOfStrings *v = it->second->first;

    findIndexesOfGivenValues(index1, index2, value1, value2, v);

    // there is no element (value1 or value2) in a set
    if (index1 == NO_RELATION || index2 == NO_RELATION)
      return false;
    else
    {
      posetRelationsArray *relationArr = it->second->second;

      // if there is an edge between value1 and value2
      if (relationArr->at(index1)[index2] == RELATION || 
      relationArr->at(index2)[index1] == RELATION)
        return false;
      else
      {
        // edge from index1(value1) to index2 (value2)
        relationArr->at(index1)[index2] = RELATION; 
        relationArr->at(index2)[index1] = RELATION_IM_LARGER;

        // now add edges that will result from transitivity
        size_t nbrOfRows = relationArr->size();

        for (index i = 0; i < nbrOfRows; i++)
        {
          if (relationArr->at(i)[index1] == RELATION && 
          relationArr->at(i)[index2] == NO_RELATION)
          {
            relationArr->at(i)[index2] = RELATION_TRANSITIVITY;
            relationArr->at(index2)[i] = RELATION_IM_LARGER;
          }
            
          else if (relationArr->at(i)[index1] == NO_RELATION && 
          relationArr->at(index2)[i] == RELATION)
          {
            relationArr->at(index1)[i] = RELATION_TRANSITIVITY;
            relationArr->at(i)[index1] = RELATION_IM_LARGER;
          }
        }

        return true;
      }
    }
  }

  return false;
};

bool poset_del(unsigned long id, char const *value1, char const *value2)
{
  if(value1 == nullptr || value2 == nullptr)
    return false;
  
  auto iter = allPosets.find(id);

  if (iter != allPosets.end())
  {
    long long index1, index2;
    vectorOfStrings *v = iter->second->first;

    findIndexesOfGivenValues(index1, index2, value1, value2, v);

    if (index1 == -1 || index2 == -1)
      return false;
    else
    {
      posetRelationsArray *relationArr = iter->second->second;

      if (relationArr->at(index1)[index2] == RELATION)
      {
        bool isOnTheLeft, isOnTheRight;

        if(relationGoodToDelete(relationArr, index1, index2, 
        isOnTheLeft, isOnTheRight))
        {
          relationArr->at(index1)[index2] = NO_RELATION;
          relationArr->at(index2)[index1] = NO_RELATION;

          if(isOnTheLeft)
            poset_del_IsOnTheLeft(relationArr, index1, index2);
          else
            poset_del_IsOnTheRight(relationArr, index1, index2);
          
          return true;
        }
      }
    }
  }
  return false;
}

bool poset_test(unsigned long id, char const *value1, char const *value2)
{
  if(value1 == nullptr || value2 == nullptr)
    return false;

  auto it = allPosets.find(id);

  if (it != allPosets.end())
  {
    long long index1;
    long long index2;
    vectorOfStrings *v = it->second->first;

    findIndexesOfGivenValues(index1, index2, value1, value2, v);

    // there is no element (value1 or value2) in a set
    if (index1 == NO_RELATION || index2 == NO_RELATION)
      return false;
    else
    {
      posetRelationsArray *p = it->second->second;

      // if there is an edge between value1 and value2 (relation or realation transitivity)
      if (p->at(index1)[index2] == RELATION || p->at(index1)[index2] == RELATION_TRANSITIVITY);
        return true;
    }
  }

  return false;
}

//DONE
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

int getValOfTwoElemRelation(const char *val1, const char *val2, poset_t const *p)
{
  vectorOfStrings *v = p->first;
  long long idx1, idx2;
  
  findIndexesOfGivenValues(idx1, idx2, val1, val2, v);
  if(idx1 == -1 || idx2 == -1)
    return 2137;
  
  return p->second->at(idx1)[idx2];
}

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

  size_t posetID2_size = 0;

  assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
  assert(allPosets[id2]->first->size() == ++posetID2_size && "insert success, but num of elem not increased\n");
  assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");

  posetID2_size += 2;

  assert(poset_insert(id2, "Z") == true && "insert into id2 poset was not succesful\n");
  assert(allPosets[id2]->first->size() == (++posetID2_size) && "insert success, but num of elem not increased");

  cout << "ADD SECTION TEST: \n";

  assert(poset_add(id2, "A", "B") == false);
  assert(poset_add(id2, "A", "X") == true);
  assert(poset_add(id2, "X", "A") == false);
  assert(poset_add(id2, "X", "Y") == true);

  cout << "TESTS PASSED\n\n";
  cout << "printing id2=" << id2 << " poset: \n\n";

  printVectorOfStrings(*allPosets[id2]->first);
  printArrOfRelations(*allPosets[id2]->second);
}

void initPoset(long long &id1, long long &id2)
{
  id1 = poset_new();
  id2 = poset_new();

  assert(poset_insert(id1, "xd") == true);
  assert(poset_insert(id1, "ALA") == true);

  assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "B") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "C") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");
}

void TEST_poset_add_remove()
{
  long long id1, id2;
  initPoset(id1, id2);
  int nbrOfPosetElem_id2 = allPosets[id2]->first->size();

  assert(poset_add(id2, "A", "B") == true);
  assert(poset_add(id2, "A", "C") == true);
  assert(poset_add(id2, "A", "B") == false);

  cout << "1) Relations added: A<B, A<C, A<B: "
       << "\n\n";
  printPoset(allPosets[id2]);

  assert(poset_add(id2, "B", "C") == true);

  cout << "2) Relations added B<C: "
       << "\n\n";
  printPoset(allPosets[id2]);

  cout << "\nposet_remove(A):\n";

  assert(poset_remove(id2, "A") == true);
  assert(allPosets[id2]->first->size() == nbrOfPosetElem_id2 - 1 && "Removal of element of poset in vec of elems didnt work\n");
  assert(allPosets[id2]->second->size() == nbrOfPosetElem_id2 - 1 && "Removal of whole row didnt work\n");
  assert(allPosets[id2]->second->at(0).size() == nbrOfPosetElem_id2 - 1 && "Removal of whole column didnt work\n");
  assert(poset_remove(id2, "A") == false);

  cout << "3) printing poset after removal:\n";
  printPoset(allPosets[id2]);
}

void DETAILED_TEST_poset_remove()
{
  long long id2 = poset_new();

  assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "B") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "C") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
  assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");

  /** A < B < C, A < X, Y < X
   *    A  B  C  X  Y
   *   ----------------
   * 0| 1  1  2  1 -1 
   * 1| 3  1  1 -1 -1
   * 2| 3  3  1 -1 -1
   * 3| 3 -1 -1  1  3
   * 4|-1 -1 -1  1  1
  */
  
  assert(poset_add(id2, "A", "B") == true);
  assert(poset_add(id2, "B", "C") == true);
  assert(poset_add(id2, "A", "X") == true);
  assert(poset_add(id2, "Y", "X") == true);

  cout << "relations: A < B < C, A < X, Y < X \n\n";
  printPoset(allPosets[id2]);

  // we remove B from poset
  assert(poset_remove(id2, "B") == true);
  printPoset(allPosets[id2]);

  // we should get
  /** A , C, A < X, Y < X
  *    A  C  X  Y
  *   ----------------
  * A| 1 -1  1 -1 
  * C|-1  1 -1 -1
  * X| 3 -1  1  3
  * Y|-1 -1  1  1
  */
  assert(getValOfTwoElemRelation("A", "X", allPosets[id2]) == 1);
  assert(getValOfTwoElemRelation("A", "C", allPosets[id2]) == -1);
  assert(getValOfTwoElemRelation("C", "A", allPosets[id2]) == -1);

  assert(poset_add(id2, "X", "C") == true);

  printPoset(allPosets[id2]);
  // we should get
  /** A < X < C, Y < X < C
  *    A  C  X  Y
  *   ----------------
  * A| 1  2  1 -1 
  * C| 3  1  3  3
  * X| 3  1  1  3
  * Y|-1  2  1  1
  */

  // C
  assert(getValOfTwoElemRelation("C", "A", allPosets[id2]) == 3);
  assert(getValOfTwoElemRelation("C", "X", allPosets[id2]) == 3);
  assert(getValOfTwoElemRelation("C", "Y", allPosets[id2]) == 3);
  // X
  assert(getValOfTwoElemRelation("X", "A", allPosets[id2]) == 3);
  assert(getValOfTwoElemRelation("X", "C", allPosets[id2]) == 1);
  assert(getValOfTwoElemRelation("X", "Y", allPosets[id2]) == 3);

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
  TEST_poset_add_remove();
  DETAILED_TEST_poset_remove();

  return 0;
}