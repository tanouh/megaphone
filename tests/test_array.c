#include "test_array.h"

#include "../src/array.h"
#include "../src/lib.h"
#include "printlib.h"
#include "testlib.h"

#include <stdio.h>
#include <stdlib.h>

static int test_push_back();
static int test_push_front();
static int test_set();
static int test_delete_at();
static int test_search();
static int test_sort();
static int test_insert_sorted();
static int test_bin_search();
static int test_clear();
static int test_free_array();

static int cmp(int *a, int *b);

void *test_array(void *d)
{
	int ret = 1;
	ret &= test_c(test_push_back, "test_push_back", print_meg);
	ret &= test_c(test_push_front, "test_push_front", print_meg);
	ret &= test_c(test_set, "test_set", print_meg);
	ret &= test_c(test_delete_at, "test_delete_at", print_meg);
	ret &= test_c(test_search, "test_search", print_meg);

	return malloc_return(ret);
}

static int cmp(int *a, int *b)
{
	if (*a == *b)
		return 0;
	return (*a < *b) ? 1 : -1;
}

static int test_push_back()
{
	struct array *a = make_array_cap(sizeof(int), 2);
	int b = 2;
	push_back(a, &b);
	b++;
	push_back(a, &b);
	b++;
	push_back(a, &b);
	int ret = 1;
	printf("%d\n", a->size);
	ret &= ASSERT(a->size == 3);
	ret &= ASSERT(a->capacity = 4);
	ret &= ASSERT(*(int *)back(a) == b);
	ret &= ASSERT(*(int *)front(a) == 2);
	free_array(a, NULL);
	return ret;
}

static int test_push_front()
{
	struct array *a = make_array_cap(sizeof(int), 2);
	int b = 2;
	push_front(a, &b);
	b++;
	push_front(a, &b);
	b++;
	push_front(a, &b);
	int ret = 1;
	ret &= ASSERT(a->size == 3);
	ret &= ASSERT(a->capacity = 4);
	ret &= ASSERT(*(int *)back(a) == 2);
	ret &= ASSERT(*(int *)front(a) == b);
	free_array(a, NULL);
	return ret;
}

static int test_set()
{
	struct array *a = make_array_cap(sizeof(int), 2);
	int b = 10;
	push_front(a, &b);
	push_front(a, &b);
	push_front(a, &b);
	b = 0;
	set(a, &b, 0);
	b++;
	set(a, &b, 1);
	b++;
	set(a, &b, 2);
	int ret = 1;
	ret &= ASSERT(a->size == 3);
	ret &= ASSERT(a->capacity = 4);
	ret &= ASSERT(*(int *)at(a, 0) == 0);
	ret &= ASSERT(*(int *)at(a, 1) == 1);
	ret &= ASSERT(*(int *)at(a, 2) == 2);
	ret &= ASSERT(set(a, &b, 3) == -1);
	ret &= ASSERT(set(a, &b, -1) == -1);
	free_array(a, NULL);
	return ret;
}

static int test_delete_at()
{
	struct array *a = make_array_cap(sizeof(int), 100);
	int b = 5;
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	push_back(a, &b);
	delete_at(a, a->size - 1, NULL);
	int ret = 1;
	ret &= ASSERT(a->size == 8);
	ret &= ASSERT(a->capacity == 50); // DEFAULT CAP
	delete_at(a, a->size - 1, NULL);
	ret &= ASSERT(a->size = 7);
	ret &= ASSERT(a->capacity == 25);
	struct array *c = make_array_cap(sizeof(int), 1);
	push_back(c, &b);
	delete_at(c, 0, NULL);
	ret &= ASSERT(c->capacity == 8);
	ret &= ASSERT(c->size == 0);
	b = 10;
	push_back(c, &b);
	ret &= ASSERT(*(int *)at(c, 0) == b);
	free_array(a, NULL);
	free_array(c, NULL);
	return ret;
}

static int test_search()
{
	struct array *a = make_array_cap(sizeof(int), 2);
	int b = 2;
	int c = b;
	push_back(a, &b);
	b++;
	push_back(a, &b);
	b++;
	push_back(a, &b);
	int ret = 1;
	ret &= ASSERT(search(a, &c, (int (*)(void *, void *))cmp) == 0);
	c = -1;
	ret &= ASSERT(search(a, &c, (int (*)(void *, void *))cmp) == -1);
	free_array(a, NULL);
	return ret;
}

static int test_sort()
{
	return 0;
}
static int test_insert_sorted()
{
	return 0;
}
static int test_bin_search()
{
	return 0;
}
static int test_clear()
{
	return 0;
}
static int test_free_array()
{
	return 0;
}
