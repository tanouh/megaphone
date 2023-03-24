#ifndef MEGAPHONE_MAP_H
#define MEGAPHONE_MAP_H
#include "array.h"

#include <stddef.h>

/*
 * INTERNAL USE ONLY: internal foreach macro.
 */
#define _MAP_FOREACH(m, key, data) \
	map_iter_start((m)); \
	while (map_iter_next((m), (key), (data)))
/**
 * Iterate through the contents of a hashmap.
 * Key and data are assigned pointers to the current hashmap entry.
 * It should be used like this :
 *\code{.c}
 * map_foreach(m, &key, &data) {
 * 	//Some code
 * }
 *\endcode
 * @param m The hashmap.
 * @param key The key buffer that will be updated at each iteration.
 * @param data The data buufer that will be updated at each iteration.
 */
#define map_foreach(m, key, data) _MAP_FOREACH((m), (key), (data))

/**
 * Iterate through the contents of a hashmap.
 * Key is an assigned pointer to the current hashmap entry.
 * It should be used like this :
 *\code{.c}
 * map_foreach_key(m, &key) {
 * 	//Some code
 * }
 *\endcode
 * @param m The hashmap.
 * @param key The key buffer that will be updated at each iteration.
 */
#define map_foreach_key(m, key) _MAP_FOREACH((m), (key), (NULL))

/**
 * Iterate through the contents of a hashmap.
 * Data is an assigned pointer to the current hashmap entry.
 * It should be used like this :
 *\code{.c}
 * map_foreach_data(m, &data) {
 * 	//Some code
 * }
 *\endcode
 * @param m The hashmap.
 * @param data The data buufer that will be updated at each iteration.
 */
#define map_foreach_data(m, data) _MAP_FOREACH((m), NULL, (data))

struct map_data;
struct map {
	size_t nkey;
	size_t ndummy;
	size_t capacity;
	struct map_data *data;
	int (*cmp)(void *, void *);
	size_t (*hash)(void *key, size_t ksize);
	size_t it;
};

/**
 * @brief Create and init the hashmap.
 * @param ksize The size of the keys
 * @param dsize
 */
struct map *make_map(int (*cmp)(void *key1, void *key2),
		     size_t (*hash)(void *, size_t));

int put_map(struct map *m, void *key, void *data, void *old_data, size_t ksize,
	    size_t dsize);
int remove_map(struct map *m, void *key, void (*free_data)(void *data),
	       size_t ksize);

int get_map(struct map *m, void *key, void *data, size_t ksize);


void free_map(struct map *m, void (*free_key)(void *),
	      void (*free_data)(void *));
void clear_map(struct map *m, void (*free_key)(void *),
	       void (*free_data)(void *));

size_t default_hash(void *key, size_t ksize);
size_t int_hash(void *key, size_t ksize);

void map_iter_start(struct map *m);
int map_iter_next(struct map *m, void *key, void *data);

#endif
