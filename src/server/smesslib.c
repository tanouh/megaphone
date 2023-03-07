#include "smesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fill_message(enum reqcode req, uint32_t id, uint16_t nbchat, uint16_t nb)
{
	char *h = malloc(MAX_HEADER);
	if (h == NULL)
		return NULL;
	char *tmp = fill_min_header(req, id);
	if (tmp == NULL) {
		free(h);
		return NULL;
	}
	memcpy(h, tmp, MIN_HEADER);
	free(tmp);
	uint16_t be = htons(nbchat);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	be = htons(nb);
	memcpy(h + 4, &be, sizeof(be));
	return h;
}

char *fill_inscription(uint16_t id)
{
	return fill_message(INSCRIPTION, id, 0, 0);
}

char *fill_push_message(uint16_t id, uint16_t nbchat)
{
	return fill_message(PUSH_MESS, id, nbchat, 0);
}

char *fill_ask_messages(uint16_t id, uint16_t nbchat, uint16_t nb)
{
	return fill_message(ASK_MESS, id, nbchat, nb);
}

char *fill_asked_message(uint16_t nbchat, char *origin, char *owner,
			 uint8_t datalen, char *data)
{
	int size = 2 * NAMELEN + sizeof(nbchat) + sizeof(datalen) + datalen;
	char *m = malloc(size);
	if (m == NULL) {
		return NULL;
	}
	uint16_t be = htons(nbchat);
	memcpy(m, &be, sizeof(be));
	memcpy(m + sizeof(nbchat), origin, NAMELEN);
	memcpy(m + sizeof(nbchat) + NAMELEN, owner, NAMELEN);
	memcpy(m + 2 * NAMELEN + sizeof(nbchat), &datalen, sizeof(datalen));
	memcpy(m + size, data, datalen);
	return m;
}

char *fill_subscribe(uint16_t id, uint16_t nbchat, uint16_t nb, char *addrmult)
{
	char *m = fill_message(SUBSCRIBE, id, nbchat, nb);
	if (realloc(m, HEADER_SERVER + ADDRMULT_LEN) == NULL) {
		free(m);
		return NULL;
	}
	memcpy(m + HEADER_SERVER, addrmult, ADDRMULT_LEN);
	return m;
}

char *fill_notification(uint16_t nbchat, char *owner, char *data, int datalen)
{
	if (datalen > NOTIFICATION_CONTENT)
		datalen = NOTIFICATION_CONTENT;
	char *m = fill_message(SUBSCRIBE, 0, nbchat, 0);
	int nsize = HEADER_SERVER - 2 + NAMELEN + NOTIFICATION_CONTENT;
	if (realloc(m, nsize) == NULL) {
		free(m);
		return NULL;
	}
	memcpy(m + HEADER_SERVER - 2, owner, NAMELEN);
	memset(m + HEADER_SERVER - 2 + NAMELEN, 0, NOTIFICATION_CONTENT);
	memcpy(m + HEADER_SERVER - 2 + NAMELEN, data, datalen);
	return m;
}

char *fill_push_file(uint16_t id, uint16_t nbchat)
{
	return fill_message(PUSH_FILE, id, nbchat, 0);
}

char *fill_pull_file(uint16_t id, uint16_t nbchat, uint16_t nb)
{
	return fill_message(PULL_FILE, id, nbchat, nb);
}

char *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       char *data)
{
	char *h = fill_min_header(req, id);
	if (realloc(h, MIN_HEADER + sizeof(nb) + datalen) == NULL) {
		return NULL;
	}
	uint16_t be = htons(nb);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	memcpy(h + MIN_HEADER + sizeof(be), data, datalen);
	return h;
}

char *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen, char *data)
{
	return fill_udp(PUSH_FILE, id, nb, datalen, data);
}

char *fill_error()
{
	return fill_message(ERROR, 0, 0, 0);
}
