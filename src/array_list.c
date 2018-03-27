#include "stdio.h"
#include "stdlib.h"
#include "array_list.h"

#define EQ_MIN(a, v1, v2) a = v1 < v2 ? v1 : v2

list *list_init() {
    list *l = malloc(sizeof(list));
    l->size = 0;
    l->list = malloc(5 * sizeof(void *));
    l->max_size = 5;
    return l;
}

list *list_init_cap(int cap) {
    list *l = malloc(sizeof(list));
    l->size = 0;
    l->list = malloc(cap * sizeof(void *));
    l->max_size = cap;
    return l;
}

void list_add(list *l, void *elem) {
    if (l->size == l->max_size) {
        list_reserve(l, 5);
    }
    l->list[l->size] = elem;
    l->size += 1;
}
void list_print(list* l){
    printf("\n");
    for (int i = 0;i<l->size; i++){
        printf("%d\t", *(int*)list_at(l, i));
    }
    printf("\n");
}
// In place non recursive merge sort.
void list_sort(list* l, int(cmp)(void*, void*)){
    for (int arr_size = 2;arr_size<l->size; arr_size*=2){
        for (int i = 0; i<l->size; i+=2*arr_size){
            int start_first = i, EQ_MIN(end_first, i+arr_size-1, l->size-1),
                            EQ_MIN(start_sec, i+arr_size, l->size-1),
                            EQ_MIN(end_sec, i+arr_size*2-1, l->size-1);
            void ** A = l->list +start_first, ** B = l->list + start_sec;
            int n = end_sec - start_sec+1, m  = end_first - start_first+1;
            for (int i=n-1; i>=0; i--){
                int j;
                void* last = A[m-1];
                for (j=m-2; j >= 0 && cmp(A[j], B[i]); j--)
                    A[j+1] = A[j];
                if (j != m-2 || cmp(last, B[i])) {
                    A[j+1] = B[i];
                    B[i] = last;
                }
            }
        }
    }
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

void* list_get(list *l, void* elem, int(compare(void*, void*))){
    for (int i = 0;i < l->size; i++){
        if (compare(elem, list_at(l, i))){
            return list_at(l, i);
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

void list_reverse(list* l){
    void* tmp;
    for (int i = 0;i<l->size/2; i++){
        tmp = list_at(l, i);
        l->list[i] = l->list[l->size-i-1];
        l->list[l->size-i-1] = tmp;
    }
}

void *list_random(list *l) {
    return list_at(l, ((unsigned int) rand()) % l->size);
}

void list_destroy(list *l) {
    free(l->list);
    free(l);
}

list* list_clone(list* l, void*(clone)(void*)){
    list* new = list_init_cap(l->size);
    for (int i = 0;i<l->size; i++){
        list_add(new, clone(list_at(l, i)));
    }
    return new;
}
