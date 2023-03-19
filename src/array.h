#ifndef MEGAPHONE_ARRAY_H
#define MEGAPHONE_ARRAY_H
#include <stddef.h>
#include <sys/types.h>

struct array {
	size_t elem_s;
	size_t capacity;
	size_t size;
	void *buf;
};

/**
 * @brief Make an array with the default capacity.
 * @param elem_s Size of an element.
 */
struct array *make_array(size_t elem_s);

/**
 * @brief Make an array with the given capacity
 * @param elem_s Size of an element.
 * @param capacity Buffer's capacity.
 */
struct array *make_array_cap(size_t elem_s, size_t capacity);

/**
 * @brief Gets the content stored at the position i of the array.
 * @param a The array.
 * @param i the position of the requested data.
 * @return The data.
 */
void *at(struct array *a, size_t i);

/**
 * @brief Gets the first data of the array.
 * Equivalent to at(a, 0)
 */
void *front(struct array *a);

/**
 * @brief Gets the last data of the array.
 * Equivalent to at(a, a->size)
 */
void *back(struct array *a);

/**
 * @brief Adds a new data at the end of the array.
 * @param a The array.
 * @param elem the element that will be added.
 * @return -1 if an error occured, 0 otherwise.
 */
int push_back(struct array *a, void *elem);
/**
 * @brief Adds a new data at the begining of the array.
 * @param a The array.
 * @param elem the element that will be added.
 * @return -1 if an error occured, 0 otherwise.
 */
int push_front(struct array *a, void *elem);
/**
 * @brief
 * @brief Adds a new data at the gitven position of the array.
 * @param a The array.
 * @param elem the element that will be added.
 * @param i The position of where the new data willbe stored.
 * @return -1 if an error occured, 0 otherwise.
 */
int set(struct array *a, void *elem, size_t i);

/**
 * @brief Delete the element of the position i and free it if the free function
 * is not NULL.
 * @param a The array.
 * @param i the position where the data will be deleted.
 * @param free_elem The data free function.
 * @return -1 if an error occured, 0 otherwise.
 */
int delete_at(struct array *a, size_t i, void (*free_elem)(void *));

/**
 * @brief Revove the given element from the list.
 * @param a The arry.
 * @param elem The element that will be removed.
 * @param cmp The data's comparison function.
 * @return -1 if an error occured, 0 otherwise.
 */
int remove_elt(struct array *a, void *elem, int (*cmp)(void *, void *));

/**
 * @brief Search the given data in the list.
 * @param a The array.
 * @param elem The element that will be researched.
 * @param cmp The data comparison function.
 * @return The index of the given data or -1 if not found.
 */
ssize_t search(struct array *a, void *elem, int (*cmp)(void *, void *));

/**
 * @brief Sorts the array acconding to the given comparison function
 * (Quicksort).
 * @param a the array.
 * @param cmp the data comparison function
 */
void sort(struct array *a, int (*cmp)(void *, void *));

/**
 * @brief Revove the given element from the list.
 * @param a The arry.
 * @param elem The element that will be removed.
 * @param cmp The data's comparison function.
 * @return -1 if an error occured, 0 otherwise.
 */
int remove_sorted(struct array *a, void *elem, int (*cmp)(void *, void *));

/**
 * @brief Insert the given element a the correct place according to the given
 * order.
 * @param a The array.
 * @param elemthe element thath will be inserted.
 * @param cmp The data comparison function.
 * @return -1 if an error occured, 0 otherwise.
 */
int insert_sorted(struct array *a, void *elem, int (*cmp)(void *, void *));

/**
 * @brief Search the given data in the list.
 * @param a The array.
 * @param elem The element that will be researched.
 * @param cmp The data comparison function.
 * @return The index of the given data.
 */
int bin_search(struct array *a, void *elem, int (*cmp)(void *, void *));

/**
 * @brief Clears and free the array content.
 * @param a The array.
 * @param free_elem The data free function.
 * @return -1 if an error occured, 0 otherwise.
 */
int clear(struct array *a, void (*free_elem)(void *));

/**
 * @brief Free the array and its content.
 * @param a the arrzay.
 * @param free_elem The data free function.
 */
void free_array(struct array *a, void (*free_elem)(void *));

#endif
