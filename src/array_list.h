//
// Created by laurent on 21/03/2018.
//

#ifndef SOKOBAN_ARRAY_LIST_H
#define SOKOBAN_ARRAY_LIST_H

typedef struct array_list list;

struct array_list {
    void **list;
    unsigned int size;
    unsigned int max_size;
};

void list_reserve(list *l, unsigned int size);

list *list_init(void);

void list_add(list *l, void *elem);

void *list_at(list *l, unsigned int index);

int list_contains(list *l, void* elem, int(*compare(void*, void*)));

void *list_last(list *l);

void list_free(list *l, void (*destructor)(void *));

void list_clear(list *l, void (*destructor)(void *));

void *list_rm_at(list *l, unsigned int index);

void list_clear_no_free(list *l);

void *list_random(list *l);

void list_destroy(list *l);


#endif //SOKOBAN_ARRAY_LIST_H
