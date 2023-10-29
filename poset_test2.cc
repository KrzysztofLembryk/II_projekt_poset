#include "poset.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#include <cassert>

namespace {
  unsigned long test() {
    unsigned long id = ::cxx::poset_new();
    ::cxx::poset_insert(id, "testowy1");
    ::cxx::poset_insert(id, "testowy2");
    ::cxx::poset_add(id, "testowy1", "testowy2");
    return id;
  }

  unsigned long id = test();
}

int main() {
  ::cxx::poset_insert(id, nullptr);
  assert(::cxx::poset_test(id, "testowy1", "testowy2"));
  assert(!::cxx::poset_test(id, "testowy2", "testowy1"));
  ::cxx::poset_delete(id);
}
