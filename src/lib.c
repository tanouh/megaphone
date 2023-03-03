#include "lib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: TESTS
char *htonx(char *h, int len)
{
	if (len <= 8) {
		char *ret = malloc(len + 1);
		if (ret == NULL)
			return NULL;
		uint32_t d;
		sscanf(h, "%x", &d);
		d = htonl(d);
		memcpy(ret, &d, len);
		ret[len] = 0;
		return ret;
	}
	int stop = (len % 2 == 0) ? 0 : 1;
	char *ret = malloc(len + stop + 1);
	if (ret == NULL)
		return NULL;
	for (int i = len - 1, j = 0; i >= stop; i -= 2, j += 2) {
		memcpy(h + i, ret + j, 2);
	}
	ret[len + stop + 1] = 0;
	if (stop != 0) {
		ret[len - 1] = '0';
		ret[len] = h[0];
	}
	return ret;
}

// TODO: tests
char *ntohx(char *h, int len)
{
	if (len <= 8) {
		char *ret = malloc(len + 1);
		if (ret == NULL)
			return NULL;
		uint32_t d;
		sscanf(h, "%x", &d);
		d = ntohl(d);
		memcpy(ret, &d, len);
		ret[len] = 0;
		return ret;
	}
	int stop = (len % 2 == 0) ? 0 : 1;
	char *ret = malloc(len + stop + 1);
	if (ret == NULL)
		return NULL;
	for (int i = len - 1, j = 0; i >= stop; i -= 2, j += 2) {
		memcpy(h + i, ret + j, 2);
	}
	ret[len + stop + 1] = 0;
	if (stop != 0) {
		ret[len - 1] = '0';
		ret[len] = h[0];
	}
}
