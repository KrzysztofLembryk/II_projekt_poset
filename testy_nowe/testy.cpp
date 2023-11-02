//Franciszek Witt
#include<bits/stdc++.h>
#include "poset.h"
using namespace std;
#define FOR(i,l,r) for(int i=(l);i<=(r);++i)
#define REP(i,n) FOR(i,0,(n)-1)
#define ssize(x) int(x.size())
#define GET_VAR_NAME(x) #x

typedef unsigned long idt;

class generator {
    private:
        mt19937 gen;
        int n;
    public:
        generator(int seed, int _n) : gen(seed), n(_n) {}
        string generate() {
            string ret = "";
            ret += char('a' + (gen() % n));
            return ret;
        }
};

unsigned poset_size(idt id, [[maybe_unused]] generator &rgen) {
    return (unsigned)cxx::poset_size(id);
}

unsigned poset_insert(idt id, generator &rgen) {
    auto val = rgen.generate();
    return (unsigned)cxx::poset_insert(id, val.c_str());
}

unsigned poset_remove(idt id, generator &rgen) {
    auto val = rgen.generate();
    return (unsigned)cxx::poset_remove(id, val.c_str());
}

unsigned poset_add(idt id, generator &rgen) {
    auto val1 = rgen.generate(), val2 = rgen.generate();
    return (unsigned)cxx::poset_add(id, val1.c_str(), val2.c_str());
}

unsigned poset_del(idt id, generator &rgen) {
    auto val1 = rgen.generate(), val2 = rgen.generate();
    return (unsigned)cxx::poset_del(id, val1.c_str(), val2.c_str());
}

unsigned poset_test(idt id, generator &rgen) {
    auto val1 = rgen.generate(), val2 = rgen.generate();
    return (unsigned)cxx::poset_test(id, val1.c_str(), val2.c_str());
}

unsigned poset_clear(idt id, [[maybe_unused]] generator &rgen) {
    cxx::poset_clear(id);
    return (unsigned)0;
}


unsigned only_this_id = 0;
// Jeżeli jeżeli ta wartość jest niezerowa, to program uruchomi tylko test o tym id.
// W przeciwnym wypadku uruchomione zostaną wszystkie testy.
unsigned global_id = 0;

void run(int q, int n, int seed, vector<function<unsigned(idt, generator&)>> tester_functions) {
    ++global_id;
    if(only_this_id and global_id != only_this_id)
        return;

    assert(n <= 'z' - 'a' + 1);
    idt id = cxx::poset_new();
    const int siz = ssize(tester_functions);
    assert(siz > 0);


    mt19937 gen(seed ^ 2137);
    generator rgen(seed, n);

    cout << global_id << ":";

    REP(i, q)
        cout << tester_functions[gen() % siz](id, rgen) << ",";
        
    cout << endl;

    cxx::poset_print(id);
    
    cxx::poset_delete(id);
}

// Tak wiem, POwinno być obiektowo ale niechcemisie.
void run_with_prep(int q, int n, int seed, vector<function<unsigned(idt, generator&)>> tester_functions) {
    ++global_id;
    if(only_this_id and global_id != only_this_id)
        return;

    assert(n <= 'z' - 'a' + 1);
    idt id = cxx::poset_new();
    const int siz = ssize(tester_functions);
    assert(siz > 0);


    mt19937 gen(seed ^ 2137);
    generator rgen(seed, n);

    cout << global_id << ":";

    REP(i, n) {
        string s = "a";
        s[0] += i;
        cxx::poset_insert(id, s.c_str());
    }
    cout << "run_with_prep - after posert_insert loop\n";
    cxx::poset_print(id);

    REP(i, q)
        cout << tester_functions[gen() % siz](id, rgen) << ",";
    cout << endl;

    cout << "run_with_prep - after tester_func loop\n";
    cxx::poset_print(id);
    
    cxx::poset_delete(id);
}

void test_1() {
}

int main() {
    FOR(siz, 4, 7)
        REP(i, 100000)
            run(30, siz, i, {poset_insert, poset_add});
            
    FOR(siz, 4, 7)
        REP(i, 100000)
            run(30, siz, i, {poset_insert, poset_add, poset_remove});

    FOR(siz, 4, 7)
        REP(i, 100000)
            run(30, siz, i, {poset_insert, poset_del, poset_add, poset_remove, poset_size, poset_test});

    FOR(siz, 3, 5)
        REP(i, 200000)
            run_with_prep(70, siz, i, {poset_add, poset_del});

    FOR(siz, 3, 5)
        REP(i, 200000)
            run_with_prep(70, siz, i, {poset_add, poset_del, poset_del, poset_del, poset_test});

}
