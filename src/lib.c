#include "lib.h"

#include "reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *fill_min_header(enum reqcode req, uint16_t id)
{
	void *h = malloc(MIN_HEADER);
	if (h == NULL)
		return NULL;
	int dcrq = reqtoi(req);
	uint16_t tmp = dcrq | (id >> REQ_BITS);
	tmp = htons(tmp);
	memcpy(h, &tmp, sizeof(tmp));
	return h;
}

int fill_buffer(const void *msg, void **buf, int size)
{
	if (buf != NULL) {
		if ((*buf = malloc(size)) == NULL)
			return -1;
		if (memcpy(buf, msg, size) == NULL) {
			free(*buf);
			*buf = NULL;
			return -1;
		}
	}
	return 0;
}

int get_min_header(const void *msg, enum reqcode *req, uint16_t *id)
{
	uint16_t buf;

	if (memcpy(&buf, msg, sizeof(buf)) == NULL)
		return -1;
	buf = ntohs(buf);
	if (req != NULL)
		*req = itoreq(REQ_MASK(buf));
	if (id != NULL)
		*id = ID_MASK(buf);
	return 0;
}
