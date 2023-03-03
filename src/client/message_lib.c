#include "message_lib.h"

#include "../codereq.h"
#include "../constants.h"
#include "../lib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 56
#define MIN_HEADER 16

char *fill_min_header(enum codereq crq, uint32_t id)
{
	char *h = malloc(HEADER_SIZE);
	if (h == NULL)
		return NULL;
	memset(h, 0, HEADER_SIZE);
	uint32_t be = htonl(to_num(crq));
	memcpy(h, &be, sizeof(be));
	be = htonl(id);
	memcpy(h + 6, &be, sizeof(be));
	return h;
}

char *fill_message(enum codereq crq, uint32_t id, int numfil, int nb,
		   uint32_t datalen, char *data)
{
	char *h = malloc(HEADER_SIZE + datalen);
	if (h == NULL)
		return NULL;
	char *tmp = fill_min_header(crq, id);
	if (tmp == NULL) {
		free(h);
		return NULL;
	}
	memcpy(h, tmp, MIN_HEADER);
	free(tmp);
	uint32_t be = htonl(numfil);
	memcpy(h + 16, &be, sizeof(be));
	be = htonl(nb);
	memcpy(h + 32, &be, sizeof(be));
	memcpy(h + 48, &datalen, sizeof(datalen));
	if (datalen > 0)
		memcpy(h + HEADER_SIZE, data, datalen);
	return h;
}

char *fill_inscription(char *nickname, int len)
{
	char *h = fill_min_header(1, 0);
	if (h == NULL)
		return NULL;
	if (realloc(h, MIN_HEADER + NAMELEN) == NULL) {
		free(h);
		return NULL;
	}
	memset(h + MIN_HEADER, '#', NAMELEN);
	memcpy(h + MIN_HEADER, nickname, len);
	return h;
}
