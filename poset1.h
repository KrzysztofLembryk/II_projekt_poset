#ifndef POSET1_H
#define POSET1_H

unsigned long poset_new(void);

void poset_delete(unsigned long id);

size_t poset_size(unsigned long id);

bool poset_insert(unsigned long id, char const *value);

bool poset_remove(unsigned long id, char const *value);

bool poset_add(unsigned long id, char const *value1, char const *value2);

bool poset_del(unsigned long id, char const *value1, char const *value2);

bool poset_test(unsigned long id, char const *value1, char const *value2);

void poset_clear(unsigned long id);

#endif