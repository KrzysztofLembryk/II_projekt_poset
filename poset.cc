#include <cassert>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "poset.h"

#define assertm(exp, msg) assert(((void)msg, exp))

using std::cerr;
using std::cin;
using std::cout;
using std::exit;
using std::pair;
using std::queue;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

using posetID_t = unsigned long;
using idx_t = size_t;
using availableIDs = queue<posetID_t>;
using poset_elem = string;
using vectorOfStrings = vector<poset_elem>;
using posetRelationsArray = vector<vector<int>>;
using poset_t = pair<vectorOfStrings *, posetRelationsArray *>;
using allPosetsMap = unordered_map<posetID_t, poset_t *>;

namespace
{
  const int RELATION = 1;
  const int NO_RELATION = -1;
  const int RELATION_TRANSITIVITY = 2;
  const int RELATION_IM_LARGER = 3;


  allPosetsMap &getAllPosets()
  {
    static allPosetsMap allPosets;
    return allPosets;
  }

  availableIDs &getAvailableIDs() {
    static availableIDs availableIDs;
    return availableIDs;
  }

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

  bool somethingIsBetweenTwoElem(posetRelationsArray *relationArr,
                                 idx_t idx1, idx_t idx2)
  {
    size_t nbrOfRows = relationArr->size();

    for (idx_t i = 0; i < nbrOfRows; i++)
    {
      if (i != idx1 && i != idx2)
      {
        if ((relationArr->at(idx1)[i] == RELATION ||
             relationArr->at(idx1)[i] == RELATION_TRANSITIVITY) &&
            (relationArr->at(i)[idx2] == RELATION ||
             relationArr->at(i)[idx2] == RELATION_TRANSITIVITY))
        {
          return true;
        }
      }
    }

    return false;
  }

  // Helper function for poset_remove.
  // It finds elements that are in relation with elem_to_remove,
  // meaning: elem1 < elem_to_remove
  // and if elem1 has transitivity relation with elem2,
  // where elem_to_remove < elem2, than relation between elem1 < elem2
  // is changed to normal relation.
  void changeRelationsTransitivity(posetRelationsArray *relArr,
                idx_t currentElem, idx_t idxOfElemToDelete, size_t nbrOfRows)
  {
    if (relArr->at(currentElem)[idxOfElemToDelete] == RELATION ||
        relArr->at(currentElem)[idxOfElemToDelete] == RELATION_TRANSITIVITY)
    {
      for (size_t j = 0; j < nbrOfRows; j++)
      {
        if ((relArr->at(idxOfElemToDelete)[j] == RELATION ||
             relArr->at(idxOfElemToDelete)[j] == RELATION_TRANSITIVITY) &&
            relArr->at(currentElem)[j] == RELATION_TRANSITIVITY)
        {
          relArr->at(currentElem)[j] = RELATION;
          relArr->at(j)[currentElem] = RELATION_IM_LARGER;
        }
      }
    }
  }

  // Helper function for poset_remove.
  // It finds elements that are larger than elem_to_delete
  // meaning elem_to_delete < elem1. Than it finds elements that
  // elem2 < elem_to_delete and if elem2 < elem1 transitivity
  // then it changes the relation.
  void changeRelationsLarger(posetRelationsArray *relArr,
                idx_t currentElem, idx_t idxOfElemToDelete, size_t nbrOfRows)
  {
    if (relArr->at(currentElem)[idxOfElemToDelete] == RELATION_IM_LARGER)
    {
      for (size_t j = 0; j < nbrOfRows; j++)
      {
        if (relArr->at(idxOfElemToDelete)[j] == RELATION_IM_LARGER &&
            relArr->at(j)[currentElem] == RELATION_TRANSITIVITY)
        {
          relArr->at(j)[currentElem] = RELATION;
          relArr->at(currentElem)[j] = RELATION_IM_LARGER;
        }
      }
    }
  }

  void checkIfElemExistInVecOfStr(vectorOfStrings *v, char const *value,
                                  idx_t &idx, bool &exist)
  {
    idx = 0;
    exist = false;
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



  void addTransitivityRelations(posetRelationsArray *relationArr,
                                idx_t index1, idx_t index2)
  {
    size_t nbrOfRows = relationArr->size();
    for (idx_t i = 0; i < nbrOfRows; i++)
    {
      if (i != index1 && i != index2) {
        if ((relationArr->at(i)[index1] == RELATION ||
            relationArr->at(i)[index1] == RELATION_TRANSITIVITY) &&
            relationArr->at(i)[index2] == NO_RELATION)
        {
          relationArr->at(i)[index2] = RELATION_TRANSITIVITY;
          relationArr->at(index2)[i] = RELATION_IM_LARGER;
          for (idx_t j = 0; j < nbrOfRows; j++)
          {
            if ((relationArr->at(index2)[j] == RELATION ||
                relationArr->at(index2)[j] == RELATION_TRANSITIVITY) &&
                relationArr->at(i)[j] == NO_RELATION)
            {
              relationArr->at(i)[j] = RELATION_TRANSITIVITY;
              relationArr->at(j)[i] = RELATION_IM_LARGER;
            }
          }
        }

        else if (relationArr->at(index1)[i] == NO_RELATION &&
                (relationArr->at(index2)[i] == RELATION ||
                relationArr->at(index2)[i] == RELATION_TRANSITIVITY))
        {
          relationArr->at(index1)[i] = RELATION_TRANSITIVITY;
          relationArr->at(i)[index1] = RELATION_IM_LARGER;
        }
      }
    }
  }


#ifdef NDEBUG
bool constexpr debug = false;
#else
bool constexpr debug = true;
#endif


#define GET_VAR_NAME(x) #x

// functions for printing debbugging messages
  string getStrErr(char const *val)
  {
    if (val == nullptr)
      return "NULL";

    string str(val);
    str = "\"" + str + "\"";
    return str;
  }

  string getPairErr(string const &s1, string const &s2 = "")
  {
    if (!s2.empty())
      return "(" + s1 + ", " + s2 + ")";
    else
      return "(" + s1 + ")";
  }

  string getPosetIdErr(posetID_t id)
  {
    string str(": poset ");
    str = str + to_string(id);
    return str;
  }

  string lastExprErr(string s = "does not exist")
  {
    return " " + s + "\n";
  }

  string invalidValErr(char const *type)
  {
    string str(type);

    return ": invalid " + str + " (NULL)\n";
  }

  string commaElemErr(string s = "element")
  {
    return ", " + s + " ";
  }

  string relationErr(string fName, posetID_t id,
  char const *value1, char const *value2)
  {
    return fName + getPosetIdErr(id) + commaElemErr("relation") +
        getPairErr(getStrErr(value1), getStrErr(value2));
  }

// functions that wrap the same debbugging cases

void posetNotExistErr(string fName, posetID_t id)
{
  cerr << fName << getPosetIdErr(id) << lastExprErr();
}

void elemNotExistErr(string fName, posetID_t id, char const *value)
{
  cerr << fName << getPosetIdErr(id) << commaElemErr() <<
        getStrErr(value) << lastExprErr();
}

void elemExistErr(string fName, posetID_t id, char const *value)
{
  cerr << fName << getPosetIdErr(id) << commaElemErr() <<
        getStrErr(value) << " already exists\n";
}

void twoValueNullErr(string fName, char const *value1, char const *value2)
{
  if (value1 == nullptr)
    cerr << fName << invalidValErr(GET_VAR_NAME(value1));
  if (value2 == nullptr)
    cerr << fName << invalidValErr(GET_VAR_NAME(value2));
}

void oneValueNullErr(string fName, char const *value)
{
  value = value;
  cerr << fName << invalidValErr(GET_VAR_NAME(value));
}

void threeArgFuncNameErr(string fName, posetID_t id, char const *value1, char const *value2)
{
  cerr << fName << getPairErr(to_string(id),
    getStrErr(value1) + ", " + getStrErr(value2)) << "\n";
}

void twoArgFuncNameErr(string fName, posetID_t id, char const *value)
{
  cerr << fName << getPairErr(to_string(id), getStrErr(value)) << "\n";
}

void oneArgFuncNameErr(string fName, posetID_t id)
{
  if (fName == "poset_new")
    cerr << fName << "()\n";
  else
    cerr << fName << getPairErr(to_string(id)) << "\n";
}

void insertedErr(string fName, posetID_t id, char const *value)
{
  cerr << fName << getPosetIdErr(id) << commaElemErr() <<
    getStrErr(value) << " inserted\n";
}

void containsErr(string fName, posetID_t id, size_t posetSize)
{
  cerr << fName << getPosetIdErr(id) << " contains " <<
       posetSize << " element(s)\n";
}

void elemRemovedErr(string fName, posetID_t id, char const *value)
{
  cerr << fName << getPosetIdErr(id) << commaElemErr() <<
      getStrErr(value) << " removed\n";
}

void isRelationAddedErr(int isAdded, string fName, posetID_t id,
char const *value1, char const *value2)
{
  if (isAdded == NO_RELATION)
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr("cannot be added");
  }
  else if (isAdded == RELATION)
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr("added");
  }
}

void isRelationDeletedErr(bool isDeleted, string fName, posetID_t id,
char const *value1, char const *value2)
{
  if(isDeleted)
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr("deleted");
  }
  else
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr("cannot be deleted");
  }
}

void relationExistsErr(bool exists, string fName, posetID_t id,
char const *value1, char const *value2)
{
  if(exists)
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr("exists");
  }
  else
  {
    cerr << relationErr(fName, id, value1, value2) <<
      lastExprErr();
  }
}

void stateOfPosetErr(string fName, posetID_t id)
{
  string str;

  if (fName == "poset_new")
    str = "created";
  else if (fName == "poset_clear")
    str = "cleared";
  else
    str = "deleted";

  cerr << fName << getPosetIdErr(id) << lastExprErr(str);
}

}

namespace cxx {

  /*
  * Creates a new poset and assigns it an id from the queue
  * of available (deleted) ids. If there are no such ids,
  * it assigns an id that is 1 greater than the previous one that
  * was not taken from queue.
  * Returns this id.
  */
  unsigned long poset_new(void)
  {
    static posetID_t nextID = 0;
    posetID_t id = 0;
    if constexpr (debug)
      oneArgFuncNameErr(string(__func__), id);

    auto &allPosets = getAllPosets();
    auto &availableIDs = getAvailableIDs();
    if (!availableIDs.empty())
    {
      id = availableIDs.front();
      availableIDs.pop();
    }
    else
    {
      id = nextID++;
    }

    poset_t *newPoset = new poset_t;
    vectorOfStrings *newVecOfPosetElem = new vectorOfStrings;
    posetRelationsArray *newRelationsArr = new posetRelationsArray;
    newPoset->first = newVecOfPosetElem;
    newPoset->second = newRelationsArr;
    allPosets.insert({id, newPoset});

    if constexpr (debug)
        stateOfPosetErr(string(__func__), id);

    return id;
  }

  /*
  * If a poset with the identifier id exists, it removes it
  * and adds its id to the queue of deleted itd.
  * Otherwise, it does nothing.
  */
  void poset_delete(unsigned long id)
  {
    if constexpr (debug)
      oneArgFuncNameErr(string(__func__), id);

    auto &allPosets = getAllPosets();

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      auto &availableIDs = getAvailableIDs();
      availableIDs.push(id);
      delete it->second->first;
      delete it->second->second;
      delete it->second;
      allPosets.erase(it);

      if constexpr (debug)
        stateOfPosetErr(string(__func__), id);
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

  }

  /*
  * If a poset with the indetifier id exists, the result
  * is the number of its elements.
  * Otherwise, it is 0.
  */
  size_t poset_size(unsigned long id)
  {
    if constexpr (debug)
      oneArgFuncNameErr(string(__func__), id);

    auto &allPosets = getAllPosets();
    size_t sizeOfPoset = 0;

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      vectorOfStrings *v = it->second->first;
      sizeOfPoset = v->size();
      if constexpr (debug)
        containsErr(string(__func__), id, sizeOfPoset);
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

    return sizeOfPoset;
  }

  /*
  * If a poset with the indetifier id exists and the element value
  * is not part of that set, it adds the element to the set,
  * otherwise it does nothing. The new element is not related to any other
  * element. The result is true when the element has been added,
  * and false otherwise.
  */
  bool poset_insert(unsigned long id, char const *value)
  {
    if constexpr (debug)
      twoArgFuncNameErr(string(__func__), id, value);

    if (value == nullptr)
    {
      if constexpr (debug)
        oneValueNullErr(string(__func__), value);

      return false;
    }

    auto &allPosets = getAllPosets();

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      vectorOfStrings *v = it->second->first;

      for (const poset_elem &str : *v)
      {
        if (str == value)
        {
          if constexpr (debug)
            elemExistErr(string(__func__), id, value);
          return false;
        }
      }
      // Adding an element to the string vector.
      poset_elem elemToAdd(value);
      v->push_back(elemToAdd);

      posetRelationsArray *p = it->second->second;
      // Adding new row.
      p->push_back(vector<int>(p->size(), NO_RELATION));
      // Adding new column.
      for (vector<int> &row : *p)
        row.push_back(NO_RELATION);
      // An element is in relation with itself.
      p->at(p->size() - 1)[p->size() - 1] = RELATION;

      if constexpr (debug)
        insertedErr(string(__func__), id, value);

      return true;
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

    return false;
  }

  /*
  * If a poset with the identifier id exists and the element value
  * belongs to that set, it removes the element from the set and also
  * removes all relations of that element; otherwise, it does nothing.
  * The result is true when the element has been removed, and false otherwise.
  */
  bool poset_remove(unsigned long id, char const *value)
  {
    if constexpr (debug)
      twoArgFuncNameErr(string(__func__), id, value);

    if (value == nullptr)
    {
      if constexpr (debug)
        oneValueNullErr(string(__func__), value);
      return false;
    }
    bool elemExists = false;
    size_t idxOfElem;
    auto &allPosets = getAllPosets();

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
          if (i != idxOfElem)
          {
            changeRelationsTransitivity(relationArr, i, idxOfElem, nbrOfRows);
            changeRelationsLarger(relationArr, i, idxOfElem, nbrOfRows);
          }
        }

        for(size_t i = 0; i < nbrOfRows; i++)
        {
            rowVec = &(relationArr->at(i));
            rowVec->erase(rowVec->begin() + idxOfElem);
        }

        relationArr->erase(relationArr->begin() + idxOfElem);

        if constexpr (debug)
          elemRemovedErr(string(__func__), id, value);

        return true;
      }
      else
      {
        if constexpr (debug)
          elemNotExistErr(string(__func__), id, value);
      }

    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

    return false;
  }

  /*
  * If a poset with the identifier id exists and the elements value1
  * and value2 belong to that set and are not related, it extends
  * the relation so that the element value1 precedes the element value2;
  * otherwise, it does nothing.
  * The result is true when the relation has been extended,
  * and false otherwise.
  */
  bool poset_add(unsigned long id, char const *value1, char const *value2)
  {
    if constexpr (debug)
      threeArgFuncNameErr(string(__func__), id, value1, value2);

    if (value1 == nullptr || value2 == nullptr)
    {
      if constexpr (debug)
        twoValueNullErr(string(__func__), value1, value2);
      return false;
    }

    auto &allPosets = getAllPosets();

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      bool foundIndex1, foundIndex2;
      size_t index1, index2;
      vectorOfStrings *v = it->second->first;

      checkIfElemExistInVecOfStr(v, value1, index1, foundIndex1);
      checkIfElemExistInVecOfStr(v, value2, index2, foundIndex2);

      if (!foundIndex1 || !foundIndex2)
      {
        if constexpr (debug)
        {
          char const *val;

          if (!foundIndex1)
            val = value1;
          else
            val = value2;

          elemNotExistErr(string(__func__), id, val);
        }
        return false;
      }
      else
      {
        posetRelationsArray *relationArr = it->second->second;

        // If there is an edge between value1 and value2,
        // meaning the elements are in relation, then do nothing.
        if (relationArr->at(index1)[index2] == RELATION ||
            relationArr->at(index2)[index1] == RELATION ||
            relationArr->at(index1)[index2] == RELATION_TRANSITIVITY ||
            relationArr->at(index2)[index1] == RELATION_TRANSITIVITY)
        {
          if constexpr (debug)
            isRelationAddedErr(NO_RELATION, string(__func__), id, value1, value2);

          return false;
        }
        else
        {
          // Adding relation (edge between value1 and value2).
          relationArr->at(index1)[index2] = RELATION;
          relationArr->at(index2)[index1] = RELATION_IM_LARGER;

          // Now adds edges that will result from transitivity.
          addTransitivityRelations(relationArr, index1, index2);

          if constexpr (debug)
            isRelationAddedErr(RELATION, string(__func__), id, value1, value2);

          return true;
        }
      }
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

    return false;
  }

  /*
  * If a poset with the identifier id exists, the elements value1 and value2
  * belong to that set, element value1 precedes element value2, and removing
  * the relation between elements value1 and value2 will not violate
  * the conditions of being a partial order, then it removes the relation
  * between these elements; otherwise, it does nothing.
  * The result is true when the relation has been changed, and false otherwise.
  */
  bool poset_del(unsigned long id, char const *value1, char const *value2)
  {
    if constexpr (debug)
      threeArgFuncNameErr(string(__func__), id, value1, value2);

    if (value1 == nullptr || value2 == nullptr)
    {
      if constexpr (debug)
        twoValueNullErr(string(__func__), value1, value2);
      return false;
    }

    auto &allPosets = getAllPosets();

    auto iter = allPosets.find(id);
    if (iter != allPosets.end())
    {
      bool foundIdx1, foundIdx2;
      idx_t index1, index2;
      vectorOfStrings *v = iter->second->first;

      checkIfElemExistInVecOfStr(v, value1, index1, foundIdx1);
      checkIfElemExistInVecOfStr(v, value2, index2, foundIdx2);

      if (!foundIdx1 || !foundIdx2)
      {
        if constexpr (debug)
        {
          if (!foundIdx1)
            elemNotExistErr(string(__func__), id, value1);
          else
            elemNotExistErr(string(__func__), id, value2);
        }
        return false;
      }
      else if (index1 == index2)
      {
        if constexpr (debug)
          isRelationDeletedErr(false, string(__func__), id, value1, value2);

        return false;
      }
      else
      {
        posetRelationsArray *relationArr = iter->second->second;

        if (relationArr->at(index1)[index2] == RELATION ||
            relationArr->at(index1)[index2] == RELATION_TRANSITIVITY)
        {
            if (!somethingIsBetweenTwoElem(relationArr, index1, index2))
            {
                relationArr->at(index1)[index2] = NO_RELATION;
                relationArr->at(index2)[index1] = NO_RELATION;

                if constexpr (debug)
                isRelationDeletedErr(true, string(__func__), id, value1, value2);

                return true;
            }
        }
        if constexpr (debug)
          isRelationDeletedErr(false, string(__func__), id, value1, value2);
      }
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }


    return false;
  }

  /*
  * If a poset with the identifier id exists, the elements value1 and value2
  * belong to that set, and element value1 precedes element value2,
  * then the result is true; otherwise, it is false."
  */
  bool poset_test(unsigned long id, char const *value1, char const *value2)
  {
    if constexpr (debug)
      threeArgFuncNameErr(string(__func__), id, value1, value2);

    if (value1 == nullptr || value2 == nullptr)
    {
      if constexpr (debug)
        twoValueNullErr(string(__func__), value1, value2);

      return false;
    }

    auto &allPosets = getAllPosets();

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      idx_t index1, index2;
      bool foundIdx1, foundIdx2;
      vectorOfStrings *v = it->second->first;

      checkIfElemExistInVecOfStr(v, value1, index1, foundIdx1);
      checkIfElemExistInVecOfStr(v, value2, index2, foundIdx2);

      if (!foundIdx1 || !foundIdx2)
      {
        if constexpr (debug)
        {
          if (!foundIdx1)
            elemNotExistErr(string(__func__), id, value1);
          else
            elemNotExistErr(string(__func__), id, value2);
        }
        return false;
      }
      else
      {
        posetRelationsArray *p = it->second->second;

        // If elements (value1 and value2) are in relation,
        // meaning if an element value1 precedes element value2.
        if (p->at(index1)[index2] == RELATION ||
          p->at(index1)[index2] == RELATION_TRANSITIVITY)
        {
          if constexpr (debug)
            relationExistsErr(true, string(__func__), id, value1, value2);

          return true;
        }
        if constexpr (debug)
          relationExistsErr(false, string(__func__), id, value1, value2);
      }
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }

    return false;
  }

  /*
  * If the poset with the identifier id exists, it removes
  * all its elements and the relations between them.
  * Otherwise, it does nothing.
  */
  void poset_clear(unsigned long id)
  {
    if constexpr (debug)
      oneArgFuncNameErr(string(__func__), id);

    auto &allPosets = getAllPosets();

    auto it = allPosets.find(id);
    if (it != allPosets.end())
    {
      vectorOfStrings *v = it->second->first;
      posetRelationsArray *p = it->second->second;
      v->clear();
      p->clear();

      if constexpr (debug)
        stateOfPosetErr(string(__func__), id);
    }
    else
    {
      if constexpr (debug)
        posetNotExistErr(string(__func__), id);
    }
  }



  void poset_print(posetID_t id)
  {
    allPosetsMap allP = getAllPosets();

    poset_t *p = allP[id];

    printVectorOfStrings(*p->first);
    printArrOfRelations(*p->second);
  }

}