#include "lib.h"

#include "constants.h"
#include "msghead.h"
#include "reqcode.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mlib = PTHREAD_MUTEX_INITIALIZER;

int fill_min_header(struct msghead h, char *buf, int bufsize)
{
	if (bufsize < MIN_HEADER)
		return -1;
	int dcrq = reqtoi(h.req);
	uint16_t tmp = dcrq | (h.id << REQ_BITS);
	tmp = htons(tmp);

	memcpy(buf, &tmp, sizeof(tmp));
	return MIN_HEADER;
}

int get_min_header(const void *msg, struct msghead *h)
{
	uint16_t buf;

	if (memcpy(&buf, msg, sizeof(buf)) == NULL)
		return -1;
	buf = ntohs(buf);
	h->req = itoreq(REQ_MASK(buf));
	h->id = ID_MASK(buf);
	return MIN_HEADER;
}

void *malloc_return(int ret)
{
	void *p = malloc(sizeof(ret));
	*(int *)p = ret;
	return p;
}
void print_s(char *msg)
{
	pthread_mutex_lock(&mlib);
	printf("%s: %s", SERVER, msg);
	pthread_mutex_unlock(&mlib);
}
void print_c(char *msg)
{
	pthread_mutex_lock(&mlib);
	printf("%s: %s", CLIENT, msg);
	pthread_mutex_unlock(&mlib);
}

int cmp_id(void *key1, void *key2)
{
	uint16_t a = *(uint16_t *)key1;
	uint16_t b = *(uint16_t *)key2;
	return (a < b)? 1 : ((a > b) ? -1 : 0);
}
