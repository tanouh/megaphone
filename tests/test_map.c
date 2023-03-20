#include "test_map.h"

#include "../src/lib.h"
#include "../src/map.h"
#include "printlib.h"
#include "string.h"
#include "testlib.h"

#include <stdio.h>

static int test_put_map();
static int test_remove_map();
static int test_get_map();
static int cmp(int *a, int *b);

void *test_map()
{
	int ret = 1;
	ret &= test_c(test_put_map, "test_put_map", print_meg);
	ret &= test_c(test_remove_map, "test_remove_map", print_meg);
	ret &= test_c(test_get_map, "test_get_map", print_meg);
	return malloc_return(ret);
}

static int test_put_map()
{
	struct map *m = make_map((int (*)(void *, void *))cmp, default_hash);
	int bool[15];
	for (int i = 0; i < 15; i++) {
		put_map(m, &i, NULL, NULL, sizeof(int), 0);
		bool[i] = 0;
	}
	int ret = 1;
	ret &= ASSERT(m->capacity == 32);
	ret &= ASSERT(m->nkey == 15);
	ret &= ASSERT(m->ndummy == 0);
	size_t key = 0;
	map_foreach_key(m, &key)
	{
		bool[key] = 1;
	}
	for (int i = 0; i < 15; i++) {
		ret &= ASSERT(bool[i]);
		if (!bool[i]) {
		}
		break;
	}
	free_map(m, NULL, NULL);
	m = make_map((int (*)(void *, void *))cmp, int_hash);
	for (int i = 0; i < 15; i++) {
		put_map(m, &i, NULL, NULL, sizeof(int), 0);
	}
	ret = 1;
	ret &= ASSERT(m->capacity == 32);
	ret &= ASSERT(m->nkey == 15);
	ret &= ASSERT(m->ndummy == 0);
	memset(bool, 0, sizeof(int) * 15);
	map_foreach_key(m, &key)
	{
		bool[key] = 1;
	}
	for (int i = 0; i < 15; i++) {
		ret &= ASSERT(bool[i]);
		if (!bool[i])
			break;
	}
	free_map(m, NULL, NULL);
	return ret;
}

static int test_remove_map()
{
	int ret = 1;
	struct map *m = make_map((int (*)(void *, void *))cmp, int_hash);
	int bool[15];
	for (int i = 0; i < 15; i++) {
		size_t key = i;
		put_map(m, &key, NULL, NULL, sizeof(int), 0);
		bool[i] = 1;
	}
	for (int i = 10; i < 15; i++) {
		remove_map(m, &i, NULL, sizeof(int));
		bool[i] = 0;
	}
	ret &= ASSERT(m->capacity == 32);
	ret &= ASSERT(m->nkey == 10);
	ret &= ASSERT(m->ndummy == 5);
	for (int i = 3; i < 7; i++) {
		remove_map(m, &i, NULL, sizeof(int));
	}

	ret &= ASSERT(m->capacity == 16);
	ret &= ASSERT(m->nkey = 3);
	ret &= ASSERT(m->ndummy == 0);
	int key;
	map_foreach_key(m, &key)
	{
		bool[key] = 1;
	}
	for (int i = 0; i < 10; i++) {
		ret &= ASSERT(bool[i]);
		if (!bool[i])
			break;
	}
	for (int i = 10; i < 15; i++) {
		ret &= ASSERT(!bool[i]);
		if (bool[i])
			break;
	}
	free_map(m, NULL, NULL);
	return ret;
}

static int test_get_map()
{
	struct map *m = make_map((int (*)(void *, void *))cmp, int_hash);
	int bool[15];
	for (int i = 0; i < 15; i++) {
		put_map(m, &i, &i, NULL, sizeof(int), sizeof(int));
		bool[i] = 0;
	}
	int key;
	map_foreach_key(m, &key)
	{
		int data;
		get_map(m, &key, &data, sizeof(int));
		bool[data] = 1;
	}
	int ret = 1;
	for (int i = 0; i < 15; i++) {
		ret &= ASSERT(bool[i]);
		if (!bool[i])
			break;
	}
	free_map(m, NULL, NULL);
	return ret;
}

static int cmp(int *a, int *b)
{
	if (a == NULL)
		return b == NULL;
	if (b == NULL)
		return (a == NULL) ? 0 : -1;

	if (*a == *b)
		return 0;
	return (*a < *b) ? 1 : -1;
}
