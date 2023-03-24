#include "map.h"

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const double HASH_A = 0.61803; /* approx of (sqrt(5)-1)/2 */
static const size_t HASH_M = 23;

#define MIN_RATIO 0.25
#define MAX_RATIO 0.75
#define DEFSIZE 16

enum status { FILLED, DUMMY, EMPTY };

struct map_data {
	size_t hkey;
	void *key;
	void *data;
	size_t ksize;
	size_t dsize;
	enum status status;
};

static const struct map_data NONE_DATA = {.ksize = 0,
					  .dsize = 0,
					  .hkey = 0,
					  .key = NULL,
					  .data = NULL,
					  .status = EMPTY};

static size_t hash1(size_t key, size_t capacity);
static size_t hash2(size_t key);
static ssize_t find(struct map *m, void *key, int flag, size_t ksize);
static int fill_data(struct map *m, struct map_data *md, void *key, void *data,
		     size_t ksize, size_t dsize);
static int resize(struct map *m, float factor);
static struct map_data *malloc_data_buf(size_t capacity);

struct map *make_map(int (*cmp)(void *key1, void *key2),
		     size_t (*hash)(void *, size_t))
{
	struct map *m = malloc(sizeof(struct map));
	if (m == NULL) {
		perror("make map");
		return NULL;
	}
	map_iter_start(m);
	m->cmp = cmp;
	m->capacity = DEFSIZE;
	m->nkey = 0;
	m->ndummy = 0;
	m->data = malloc_data_buf(m->capacity);
	m->hash = (hash == NULL) ? default_hash : hash;
	if (m->data == NULL) {
		perror("make map");
		free(m);
		return NULL;
	}
	return m;
}

int put_map(struct map *m, void *key, void *data, void *old_data, size_t ksize,
	    size_t dsize)
{
	int ret = 0;
	ssize_t ind = find(m, key, 1, ksize);
	if (ind == -1)
		return -1;
	struct map_data *md = m->data + ind;
	switch (md->status) {
	case EMPTY:
		m->nkey++;
		break;
	case DUMMY:
		m->ndummy--;
		m->nkey++;
		break;
	case FILLED:
		if (md->data == NULL)
			break;
		if (old_data != NULL) {
			memcpy(old_data, md->data, md->dsize);
			ret = 1; /* old data is set */
		}
		free(md->data);
		break;
	default:
		perror("Unknown status");
		return -1;
	}
	if (fill_data(m, md, key, data, ksize, dsize) != 0)
		return -1;
	if (m->nkey + m->ndummy > MAX_RATIO * m->capacity) {
		int ratio = (m->ndummy > m->nkey) ? 1 : 2;
		return resize(m, ratio);
	}
	return ret;
}

void map_iter_start(struct map *m)
{
	m->it = 0;
}
int map_iter_next(struct map *m, void *key, void *data)
{
	while (m->it < m->capacity && m->data[m->it].status != FILLED)
		m->it++;
	if (m->it >= m->capacity)
		return 0;
	struct map_data md = m->data[m->it];
	if (key != NULL)
		memcpy(key, md.key, md.ksize);
	if (data != NULL && md.data != NULL)
		memcpy(data, md.data, md.dsize);
	m->it++;
	return 1;
}

int get_map(struct map *m, void *key, void *data, size_t ksize)
{
	ssize_t ind = 0;
	if ((ind = find(m, key, 0, ksize)) < 0)
		return -1;
	struct map_data md = m->data[(size_t)ind];
	if (data != NULL && m->data[(size_t)ind].data != NULL)
		memcpy(data, m->data[(size_t)ind].data, md.dsize);
	return 0;
}

int remove_map(struct map *m, void *key, void (*free_data)(void *data),
	       size_t ksize)
{
	ssize_t ind = find(m, key, 0, ksize);
	if (ind < 0)
		return -1;
	struct map_data *md = m->data + (size_t)ind;
	if (md->data != NULL) {
		if (free_data != NULL)
			free_data(md->data);
		free(md->data);
	}
	free(md->key);
	md->data = NULL;
	md->key = NULL;
	md->status = DUMMY;
	m->ndummy++;
	m->nkey--;
	if ((m->nkey < MIN_RATIO * m->capacity)) {
		float ratio = (m->ndummy > m->nkey)? 1 : 0.5;
		resize(m, ratio);
	}
	return 0;
}

void clear_map(struct map *m, void (*free_key)(void *),
	       void (*free_data)(void *))
{
	for (size_t i = 0; i < m->capacity; i++) {
		struct map_data *md = m->data + i;
		if (md->status == FILLED) {
			if (free_key != NULL)
				free_key(md->key);
			if (free_data != NULL)
				free_data(md->data);
			free(md->key);
			free(md->data);
		}
	}
	struct map_data *def =
		malloc_data_buf(DEFSIZE * sizeof(struct map_data));
	if (def == NULL) {
		perror("clear_map");
		return;
	}
	free(m->data);
	m->data = def;
	m->capacity = DEFSIZE;
	map_iter_start(m);
	m->ndummy = 0;
	m->ndummy = 0;
}
void free_map(struct map *m, void (*free_key)(void *),
	      void (*free_data)(void *))
{
	for (size_t i = 0; i < m->capacity; i++) {
		struct map_data *md = m->data + i;
		if (md->status == FILLED) {
			if (free_key != NULL)
				free_key(md->key);
			if (free_data != NULL && md->data != NULL)
				free_data(md->data);
			free(md->key);
			free(md->data);
		}
	}
	free(m->data);
	free(m);
}

size_t default_hash(void *key, size_t ksize)
{
	size_t h = 0;
	char *p = (char *)key;
	for (size_t i = 0; i < ksize; i++) {
		h += (('a' + p[i]) % 'Z') * ((ksize - i) << 5);
	}
	return h;
}

size_t int_hash(void *key, size_t ksize)
{
	size_t incr = 0;
	size_t s = sizeof(size_t);
	size_t h = 0;
	for (size_t i = 0; i < ksize; i += incr) {
		incr = s > ksize ? ksize : s;
		size_t x = 0;
		memcpy(&x, key + i, incr);
		double ax = HASH_A * x;
		h += (size_t)(HASH_M * (ax - (size_t)(ax)));
	}
	return h;
}

static size_t hash1(size_t h, size_t capacity)
{
	return (size_t)(((double)h * HASH_A) / (double)capacity);
}

static size_t hash2(size_t hash)
{
	return 2 * hash + 1;
}

static ssize_t find(struct map *m, void *key, int flag, size_t ksize)
{
	size_t h = m->hash(key, ksize);
	size_t ind = hash1(h, m->capacity) % m->capacity;
	size_t step = hash2(h) % m->capacity;
	ssize_t dummy = -1;
	for (size_t i = 0; i < m->capacity; i++) {
		switch (m->data[ind].status) {
		case EMPTY:
			if (dummy == -1)
				dummy = ind;
			return (flag) ? dummy : -1;
		case DUMMY:
			if (dummy == -1)
				dummy = ind;
			break;
		default:
			if (m->data[ind].hkey == h &&
			    m->cmp(m->data[ind].key, key) == 0)
				return ind;
			break;
		}
		ind = (ind + step) % m->capacity;
	}
	return (flag) ? dummy : -1;
}

static int fill_data(struct map *m, struct map_data *md, void *key, void *data,
		     size_t ksize, size_t dsize)
{
	md->hkey = m->hash(key, ksize);
	if (md->key == NULL)
		md->key = malloc(ksize);
	if (md->key == NULL) {
		perror("fill_data");
		return -1;
	}
	memcpy(md->key, key, ksize);
	if (data != NULL) {
		md->data = malloc(dsize);
		if (md->data == NULL) {
			free(md->key);
			md->key = NULL;
			perror("fill_data");
			return -1;
		}
		memcpy(md->data, data, dsize);
	}
	md->status = FILLED;
	md->ksize = ksize;
	md->dsize = dsize;
	return 0;
}

static int resize(struct map *m, float factor)
{
	size_t newsize = m->capacity * factor;
	newsize = (newsize < DEFSIZE) ? DEFSIZE : newsize;
	struct map_data *old = m->data;
	size_t oldcap = m->capacity;
	m->data = malloc_data_buf(newsize);
	if (m->data == NULL) {
		perror("resize");
		m->data = old;
		return -1;
	}
	m->capacity = newsize;
	m->nkey = 0;
	m->ndummy = 0;
	map_iter_start(m);
	for (size_t i = 0; i < oldcap; i++) {
		if (old[i].status == FILLED) {
			if (put_map(m, old[i].key, old[i].data, NULL,
				    old[i].ksize, old[i].dsize) != 0) {
				perror("resize");
				free(m->data);
				m->data = old;
				return -1;
			}
			free(old[i].key);
			free(old[i].data);
		}
	}
	free(old);
	return 0;
}

static struct map_data *malloc_data_buf(size_t capacity)
{
	struct map_data *data = malloc(sizeof(struct map_data) * capacity);
	if (data == NULL) {
		perror("make map");
		return NULL;
	}
	for (size_t i = 0; i < capacity; i++) {
		memcpy(data + i, &NONE_DATA, sizeof(struct map_data));
	}
	return data;
}
