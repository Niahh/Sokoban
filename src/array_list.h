//
// Created by pierre on 21/03/2018.
//

#ifndef SOKOBAN_ARRAY_LIST_H
#define SOKOBAN_ARRAY_LIST_H

typedef struct array_list list;
/**
 * A generic array list.
 */
struct array_list {
    // the array
    void **list;
    // the size of the array
    unsigned int size;
    // the current max size of the list
    unsigned int max_size;
};
/**
 * Reserves some memory, extending the list.
 * @param l : the list.
 * @param size : the size to add to the list.
 */
void list_reserve(list *l, unsigned int size);

/**
 * Creates an empty list.
 * The max size of this list will be 5, see list_init_cap for more options.
 * @return The created list.
 */
list *list_init(void);

/**
 * Creates an empty list with the given memory size.
 * @return The created list.
 */
list *list_init_cap(int cap);

/**
 * Adds an element to the list, the element will be added to the end of the list.
 * @param l : the list.
 * @param elem : The element to add.
 */
void list_add(list *l, void *elem);

/**
 * Returns the element at the given position in the list.
 * @param l : the list
 * @param index : the position of the element.
 * @return The element.
 */
void *list_at(list *l, unsigned int index);

/**
 * Sorts the list using an in place merge sort.
 * @param l : the list.
 * @param cmp : A function to compare the elements. The function must take a and b and returns 1 if a > b
 */
void list_sort(list* l, int(cmp)(void*, void*));

/**
 * Checks that the list contains the given element.
 * @param l : the list.
 * @param elem : the element to compare against.
 * @param compare : the function to compare, the function will take elem and B and return 1 if they match.
 * @return true if the element exist false otherwise.
 */
int list_contains(list *l, void* elem, int(compare(void*, void*)));

/**
 * Checks that the list contains an element matching the given element and return it.
 * @param l : the list.
 * @param elem : the element to compare against.
 * @param compare : the function to compare, the function will take elem and B and return 1 if they match.
 * @return The element found, or NULL if it was not found.
 */
void* list_get(list *l, void* elem, int(compare(void*, void*)));

/**
 * Returns the last element of the list.
 * @param l : the list.
 * @return The last element.
 */
void *list_last(list *l);

/**
 * Free the list and all its elements using the given constructor.
 * @param l : the list.
 * @param destructor : The function to free/destroy an element.
 */
void list_free(list *l, void (*destructor)(void *));

/**
 * Clears the list using the given destructor but does not free the list.
 * @param l : the list.
 * @param destructor : The function to free/destroy an element.
 */
void list_clear(list *l, void (*destructor)(void *));

/**
 * Removes the element at the given position from the list and returns it.
 * @param l : the list.
 * @param index : the element to remove the list from.
 * @return The element.
 */
void *list_rm_at(list *l, unsigned int index);

/**
 * Clear the list without freeing the elements inside it.
 * @param l : the list.
 */
void list_clear_no_free(list *l);

/**
 * Returns a random element from the list.
 * @param l : the list.
 * @return The randomly chosen element.
 */
void *list_random(list *l);

/**
 * Reverses the list order.
 * @param l : the list.
 */
void list_reverse(list* l);

/**
 * Destroys the list without freeing its elements.
 * @param l : the list.
 */
void list_destroy(list *l);

/**
 * Returns a new list filled with clones of the elements of the current list.
 * @param l : the list.
 * @return the cloned list.
 */
list* list_clone(list* l, void*(*)(void*));

#endif //SOKOBAN_ARRAY_LIST_H
