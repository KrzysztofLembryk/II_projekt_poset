#include "poset.h"

using namespace cxx;

void test400100()
{
    unsigned int id = poset_new();
    poset_add(id, "b", "d");
    poset_insert(id, "b");
    poset_remove(id, "a");
    poset_insert(id, "b");
    poset_add(id, "a", "c");
    poset_insert(id, "a");
    poset_add(id, "b", "a");
    poset_remove(id, "b");
    poset_add(id, "d", "c");
    poset_remove(id, "d");
    poset_add(id, "d", "b");
    poset_add(id, "b", "a");
    poset_add(id, "b", "d");
    poset_insert(id, "c");
    poset_insert(id, "d");
    poset_add(id, "a", "d");
    poset_add(id, "c", "a");
    poset_add(id, "a", "a");
    poset_insert(id, "a");
    poset_remove(id, "a");
    /*poset_remove(id, "b");
    poset_insert(id, "d");
    poset_insert(id, "d");
    poset_remove(id, "d");
    poset_add(id, "c", "d");
    poset_remove(id, "d");
    poset_remove(id, "c");
    poset_remove(id, "b");
    poset_remove(id, "c");
    poset_insert(id, "a");
*/
}

int main()
{
    test400100();

    return 0;
}