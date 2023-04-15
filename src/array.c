#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFSIZE 32
#define MINCAP 8
#define MIN_RATIO 0.25

static int resize(struct array *a, double factor);

static int partition(struct array *a, size_t start, size_t end,
		     int (*cmp)(void *, void *));
static void sort_aux(struct array *a, size_t start, size_t end,
		     int (*cmp)(void *, void *));
struct array *make_array_cap(size_t elem_size, size_t capacity)
{
	struct array *a = malloc(sizeof(struct array));
	if (a == NULL) {
		perror("make_array");
		return NULL;
	}
	a->size = 0;
	a->capacity = capacity;
	a->elem_s = elem_size;
	if ((a->buf = calloc(capacity, elem_size)) == NULL) {
		perror("make_array");
		free(a);
		return NULL;
	}
	return a;
}

struct array *make_array(size_t elem_size)
{
	return make_array_cap(elem_size, DEFSIZE);
}

void *at(struct array *a, size_t i)
{
	if (a == NULL || i >= a->size)
		return NULL;
	return ((void *)(char *)a->buf + i * a->elem_s);
}

void *front(struct array *a)
{
	return at(a, 0);
}

void *back(struct array *a)
{
	return at(a, a->size - 1);
}

int push_back(struct array *a, void *e)
{
	if (a->size + 1 >= a->capacity && resize(a, 2) != 0) {
		perror("push_back");
		return -1;
	}
	memcpy((char *)a->buf + a->size * a->elem_s, e, a->elem_s);
	a->size++;
	return 0;
}
int push_front(struct array *a, void *e)
{
	if (a->size + 1 >= a->capacity && resize(a, 2)) {
		perror("push_back");
		return -1;
	}
	memmove((char *)a->buf + a->elem_s, a->buf, a->elem_s * a->size);
	memcpy(a->buf, e, a->elem_s);
	a->size++;
	return 0;
}

int set(struct array *a, void *e, size_t i)
{
	if (i >= a->size)
		return -1;
	memcpy((char *)a->buf + i * a->elem_s, e, a->elem_s);
	return 0;
}

int delete_at(struct array *a, size_t i, void (*free_elem)(void *))
{
	void *e = at(a, i);
	if (e == NULL)
		return -1;
	if (free_elem != NULL)
		free_elem(e);
	char *p = (char *)a->buf + i * a->elem_s;
	memmove(p + a->elem_s, p, (a->size - i) * a->elem_s);
	a->size--;
	if (a->size < a->capacity * MIN_RATIO) {
		resize(a, 0.5);
	}
	return 0;
}

int remove_elt(struct array *a, void *elem, int (*cmp)(void *, void *))
{
	return delete_at(a, search(a, elem, cmp), NULL);
}

ssize_t search(struct array *a, void *elem, int (*cmp)(void *, void *))
{
	size_t i = 0;
	for (; i < a->size; i++) {
		void *e = at(a, i);
		if (cmp(elem, e) == 0)
			break;
	}
	return (i == a->size) ? -1 : (ssize_t)i;
}

void sort(struct array *a, int (*cmp)(void *, void *))
{
	srand(time(NULL));
	sort_aux(a, 0, a->size, cmp);
}

int remove_sorted(struct array *a, void *elem, int (*cmp)(void *, void *))
{
	return delete_at(a, bin_search(a, elem, cmp), NULL);
}

int insert_sorted(struct array *a, void *elem, int (*cmp)(void *, void *))
{
	size_t l = 0;
	size_t r = a->size;
	ssize_t i = -1;
	while (l > r && i == -1) {
		int m = (l + r) / 2;
		switch (cmp(elem, at(a, m))) {
		case -1:
			l = m + 1;
			break;
		case 1:
			r = m - 1;
			break;
		default:
			i = m;
			break;
		}
	}
	if (i == -1) {
		switch (cmp(elem, at(a, l))) {
		case -1:
			i = r + 1;
			break;
		case 1:
			i = l + 1;
			break;
		default:
			i = l;
			break;
		}
	}
	if (a->size + 1 >= a->capacity && resize(a, 2) != 0) {
		perror("inseert_sorted");
		return -1;
	}
	char *p = (char *)a->buf + i * a->elem_s;
	memmove(p + a->elem_s, p, a->elem_s * (a->size - i));
	memcpy(p, elem, a->elem_s);
	return 0;
}

int bin_search(struct array *a, void *elem, int (*cmp)(void *, void *))
{
	size_t l = 0;
	size_t r = a->size;
	while (l >= r) {
		int m = (l + r) / 2;
		switch (cmp(elem, at(a, m))) {
		case -1:
			l = m + 1;
			break;
		case 1:
			r = m - 1;
			break;
		default:
			return m;
			break;
		}
	}
	return -1;
}

int clear(struct array *a, void (*free_elem)(void *))
{
	for (size_t i = 0; i < a->size; i++) {
		void *p = ((void *)(char *)a->buf + i * a->elem_s);
		if (free_elem != NULL)
			free_elem(p);
	}
	void *tmp = realloc(a->buf, DEFSIZE);
	if (tmp == NULL) {
		perror("clear");
		a->buf = NULL;
		return -1;
	}
	a->buf = tmp;
	a->capacity = DEFSIZE;
	a->size = 0;
	return 0;
}

void free_array(struct array *a, void (*free_elem)(void *))
{
	clear(a, free_elem);
	free(a->buf);
	free(a);
}

static void sort_aux(struct array *a, size_t start, size_t end,
		     int (*cmp)(void *, void *))
{
	if (end - start < 2)
		return;
	size_t p = partition(a, start, end, cmp);
	sort_aux(a, start, p, cmp);
	sort_aux(a, p + 1, end, cmp);
}

static void swap(struct array *a, size_t i, size_t j)
{
	void *tmp = malloc(a->elem_s);
	memcpy(tmp, at(a, i), a->elem_s);
	set(a, at(a, j), i);
	set(a, tmp, j);
	free(tmp);
}

static int partition(struct array *a, size_t start, size_t end,
		     int (*cmp)(void *, void *))
{
	size_t rd = start + (rand() % (end - start));
	swap(a, start, rd);
	void *pivot = malloc(a->elem_s);
	memcpy(pivot, at(a, start), a->elem_s);
	size_t l = start + 1;
	size_t r = end - 1;
	while (l <= r) {
		if (cmp(at(a, l), pivot) >= 0)
			l++;
		else if (cmp(at(a, r), pivot) <= 0)
			r--;
		else
			swap(a, r, l);
	}
	set(a, at(a, r), start);
	set(a, pivot, r);
	free(pivot);
	return r;
}

static int resize(struct array *a, double factor)
{
	size_t newcap = a->capacity * factor;
	if (newcap < MINCAP)
		newcap = MINCAP;
	void *tmp = realloc(a->buf, a->elem_s * newcap);
	if (tmp == NULL) {
		perror("resize");
		return -1;
	}
	a->buf = tmp;
	a->capacity = newcap;
	return 0;
}
