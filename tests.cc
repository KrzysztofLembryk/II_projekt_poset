// #include "poset.h"

// int getValOfTwoElemRelation(const char *val1, const char *val2, poset_t const *p)
// {
//   vectorOfStrings *v = p->first;
//   size_t idx1, idx2;
//   bool foundIdx1, foundIdx2;

//   checkIfElemExistInVecOfStr(v, val1, idx1, foundIdx1);
//   checkIfElemExistInVecOfStr(v, val2, idx2, foundIdx2);
  
//   if (!foundIdx1 || !foundIdx2)
//     return 2137;

//   return p->second->at(idx1)[idx2];
// }

// void TEST_poset_new_delete_insert_add()
// {
//   cout << "----- TEST_poset_new_delete_insert -----\n";
//   cout << "NEW/DELETE SECTION TEST: \n";

//   unsigned long id1 = poset_new();
//   unsigned long id2 = poset_new();
//   unsigned long id3 = poset_new();

//   assert(allPosets.size() == 3 && "allPosets.size is not equal 3, creating posets went wrong\n");
//   poset_delete(id1);
//   assert(allPosets.size() == 2 && "allPosets.size is not equal 2, deleting went wrong\n");

//   unsigned long id4 = poset_new();

//   assert(id2 == 1 && "id2 should equal 1\n");

//   poset_delete(id3);
//   poset_delete(id4);

//   cout << "INSERT SECTION TEST: \n";

//   assert(poset_insert(id1, "xd") == false && "poset id1 does not exist, but inserting was a success - WRONG\n");

//   size_t posetID2_size = 0;

//   assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
//   assert(allPosets[id2]->first->size() == ++posetID2_size && "insert success, but num of elem not increased\n");
//   assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");

//   posetID2_size += 2;

//   assert(poset_insert(id2, "Z") == true && "insert into id2 poset was not succesful\n");
//   assert(allPosets[id2]->first->size() == (++posetID2_size) && "insert success, but num of elem not increased");

//   cout << "ADD SECTION TEST: \n";

//   assert(poset_add(id2, "A", "B") == false);
//   assert(poset_add(id2, "A", "X") == true);
//   assert(poset_add(id2, "X", "A") == false);
//   assert(poset_add(id2, "X", "Y") == true);

//   cout << "TESTS PASSED\n\n";
//   cout << "printing id2=" << id2 << " poset: \n\n";

//   printVectorOfStrings(*allPosets[id2]->first);
//   printArrOfRelations(*allPosets[id2]->second);
// }

// void initPoset(idx_t &id1, idx_t &id2)
// {
//   id1 = poset_new();
//   id2 = poset_new();

//   assert(poset_insert(id1, "xd") == true);
//   assert(poset_insert(id1, "ALA") == true);

//   assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "B") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "C") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");
// }

// void TEST_poset_add_remove()
// {
//   idx_t id1, id2;
//   initPoset(id1, id2);
//   size_t nbrOfPosetElem_id2 = allPosets[id2]->first->size();

//   assert(poset_add(id2, "A", "B") == true);
//   assert(poset_add(id2, "A", "C") == true);
//   assert(poset_add(id2, "A", "B") == false);

//   cout << "1) Relations added: A<B, A<C, A<B: "
//        << "\n\n";
//   printPoset(allPosets[id2]);

//   assert(poset_add(id2, "B", "C") == true);

//   cout << "2) Relations added B<C: "
//        << "\n\n";
//   printPoset(allPosets[id2]);

//   cout << "\nposet_remove(A):\n";

//   assert(poset_remove(id2, "A") == true);
//   assert(allPosets[id2]->first->size() == nbrOfPosetElem_id2 - 1 && "Removal of element of poset in vec of elems didnt work\n");
//   assert(allPosets[id2]->second->size() == nbrOfPosetElem_id2 - 1 && "Removal of whole row didnt work\n");
//   assert(allPosets[id2]->second->at(0).size() == nbrOfPosetElem_id2 - 1 && "Removal of whole column didnt work\n");
//   assert(poset_remove(id2, "A") == false);

//   cout << "3) printing poset after removal:\n";
//   printPoset(allPosets[id2]);
// }

// void DETAILED_TEST_poset_remove()
// {
//   posetID_t id2 = poset_new();

//   assert(poset_insert(id2, "A") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "B") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "C") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "X") == true && "insert into id2 poset was not succesful\n");
//   assert(poset_insert(id2, "Y") == true && "insert into id2 poset was not succesful\n");

//   /** A < B < C, A < X, Y < X
//    *    A  B  C  X  Y
//    *   ----------------
//    * 0| 1  1  2  1 -1
//    * 1| 3  1  1 -1 -1
//    * 2| 3  3  1 -1 -1
//    * 3| 3 -1 -1  1  3
//    * 4|-1 -1 -1  1  1
//    */

//   assert(poset_add(id2, "A", "B") == true);
//   assert(poset_add(id2, "B", "C") == true);
//   assert(poset_add(id2, "A", "X") == true);
//   assert(poset_add(id2, "Y", "X") == true);

//   cout << "relations: A < B < C, A < X, Y < X \n\n";
//   printPoset(allPosets[id2]);

//   // we remove B from poset
//   assert(poset_remove(id2, "B") == true);
//   printPoset(allPosets[id2]);

//   // we should get
//   /** A , C, A < X, Y < X
//    *    A  C  X  Y
//    *   ----------------
//    * A| 1 -1  1 -1
//    * C|-1  1 -1 -1
//    * X| 3 -1  1  3
//    * Y|-1 -1  1  1
//    */
//   assert(getValOfTwoElemRelation("A", "X", allPosets[id2]) == 1);
//   assert(getValOfTwoElemRelation("A", "C", allPosets[id2]) == -1);
//   assert(getValOfTwoElemRelation("C", "A", allPosets[id2]) == -1);

//   assert(poset_add(id2, "X", "C") == true);

//   printPoset(allPosets[id2]);
//   // we should get
//   /** A < X < C, Y < X < C
//    *    A  C  X  Y
//    *   ----------------
//    * A| 1  2  1 -1
//    * C| 3  1  3  3
//    * X| 3  1  1  3
//    * Y|-1  2  1  1
//    */

//   // C
//   assert(getValOfTwoElemRelation("C", "A", allPosets[id2]) == 3);
//   assert(getValOfTwoElemRelation("C", "X", allPosets[id2]) == 3);
//   assert(getValOfTwoElemRelation("C", "Y", allPosets[id2]) == 3);
//   // X
//   assert(getValOfTwoElemRelation("X", "A", allPosets[id2]) == 3);
//   assert(getValOfTwoElemRelation("X", "C", allPosets[id2]) == 1);
//   assert(getValOfTwoElemRelation("X", "Y", allPosets[id2]) == 3);
// }

// void test_peczar1()
// {
//   unsigned long p1 = poset_new();

//   assert(poset_size(p1) == 0);
//   assert(poset_size(p1 + 1) == 0);
//   assert(!poset_insert(p1, NULL));
//   assert(poset_insert(p1, "A"));
//   assert(poset_test(p1, "A", "A"));
//   assert(!poset_insert(p1, "A"));
//   assert(!poset_insert(p1 + 1, "B"));
//   assert(poset_size(p1) == 1);
//   assert(!poset_remove(p1 + 1, "A"));
//   assert(poset_remove(p1, "A"));
//   assert(!poset_remove(p1, "A"));
//   assert(poset_insert(p1, "B"));
//   assert(poset_insert(p1, "C"));
//   assert(poset_add(p1, "B", "C"));
//   assert(!poset_remove(p1, "A"));
//   assert(!poset_add(p1, NULL, "X"));
//   assert(!poset_del(p1, NULL, "X"));
//   assert(!poset_test(p1, NULL, "X"));
//   assert(!poset_add(p1, "X", NULL));
//   assert(!poset_del(p1, "X", NULL));
//   assert(!poset_test(p1, "X", NULL));
//   assert(!poset_add(p1, NULL, NULL));
//   assert(!poset_del(p1, NULL, NULL));
//   assert(!poset_test(p1, NULL, NULL));
//   assert(!poset_add(p1, "C", "D"));
//   assert(!poset_add(p1, "D", "C"));
//   assert(!poset_add(p1, "D", "D"));
//   assert(!poset_add(p1, "E", "D"));
//   assert(!poset_del(p1, "C", "D"));
//   assert(!poset_del(p1, "D", "C"));
//   assert(!poset_del(p1, "D", "D"));
//   assert(!poset_del(p1, "E", "D"));
//   assert(!poset_test(p1, "C", "D"));
//   assert(!poset_test(p1, "D", "C"));
//   assert(!poset_test(p1, "D", "D"));
//   assert(!poset_test(p1, "E", "D"));
//   assert(!poset_add(p1 + 1, "C", "D"));
//   assert(!poset_del(p1 + 1, "C", "D"));
//   assert(!poset_test(p1 + 1, "C", "D"));
//   poset_clear(p1);
//   poset_clear(p1 + 1);
//   assert(poset_insert(p1, "E"));
//   assert(poset_insert(p1, "F"));
//   assert(poset_insert(p1, "G"));
//   assert(poset_add(p1, "E", "F"));
//   assert(!poset_add(p1, "E", "F"));
//   assert(!poset_add(p1, "F", "E"));
//   assert(poset_test(p1, "E", "F"));
//   assert(!poset_test(p1, "F", "E"));
//   assert(poset_add(p1, "F", "G"));
//   assert(poset_test(p1, "E", "G"));
//   assert(!poset_del(p1, "E", "G"));
//   assert(poset_del(p1, "E", "F"));
//   assert(!poset_del(p1, "E", "F"));
//   assert(!poset_del(p1, "G", "F"));
//   assert(!poset_del(p1, "G", "G"));
//   assert(poset_size(p1) == 3);
//   poset_delete(p1);
//   poset_delete(p1);
//   poset_delete(p1 + 1);
// }

// // --------------------------------

// int main()
// {
//   // TEST_poset_new_delete_insert_add();
//   // TEST_poset_add_remove();
//   // DETAILED_TEST_poset_remove();
//   test_peczar1();

//   return 0;
// }