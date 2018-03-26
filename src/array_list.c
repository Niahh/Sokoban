#include "stdio.h"
#include "stdlib.h"
#include "array_list.h"

list *list_init() {
    list *l = malloc(sizeof(list));
    l->size = 0;
    l->list = malloc(5 * sizeof(void *));
    l->max_size = 5;
    return l;
}

void list_add(list *l, void *elem) {
    if (l->size == l->max_size) {
        list_reserve(l, 5);
    }
    l->list[l->size] = elem;
    l->size += 1;
}

void list_reserve(list *l, unsigned int size) {
    l->list = realloc(l->list, (size + l->max_size) * sizeof(void *));
    l->max_size += size;
}

void *list_at(list *l, unsigned int index) {
    if (index >= l->size) {
        printf("error at : \n    list_at : index out of bound\n      list size is %u but index is %u\n\n     exiting \n",
               l->size, index);
        list *a = (void *) (12);
        a->size++;
        exit(0);
    }
    return l->list[index];
}

void *list_last(list *l) {
    if (l->size == 0) {
        printf("no last element on empty list\n exiting");
        exit(0);
    }
    return l->list[l->size - 1];
}

void list_free(list *l, void (*destructor)(void *)) {
    list_clear(l, destructor);
    free(l->list);
    free(l);
}


int list_contains(list *l, void* elem, int(compare(void*, void*))){
    for (int i = 0;i < l->size; i++){
        if (compare(elem, list_at(l, i))){
            return 1;
        }
    }
    return 0;
}


void list_clear(list *l, void (*destructor)(void *)) {
    for (unsigned int i = 0; i < l->size; i++) {
        (*destructor)(l->list[i]);
    }
    l->size = 0;
}

void list_clear_no_free(list *l) {
    l->size = 0;
}

void *list_rm_at(list *l, unsigned int index) {
    if (index >= l->size) {
        printf("error at : \n    list_remove_at : index out of bound\n      list size is %u but index is %u\n\n     exiting \n",
               l->size, index);
        exit(0);
    }
    void *temp = l->list[index];
    l->size -= 1;
    for (unsigned int i = index; i < l->size; i++) {
        l->list[i] = l->list[i + 1];
    }
    return temp;
}


void *list_random(list *l) {
    return list_at(l, ((unsigned int) rand()) % l->size);
}

void list_destroy(list *l) {
    free(l->list);
    free(l);
}
