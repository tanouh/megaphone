#include "message_lib.h"

#include "../reqcode.h"
#include "../constants.h"
#include "../lib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fill_min_header(enum reqcode req, uint16_t id)
{
	char *h = malloc(MIN_HEADER);
	if (h == NULL)
		return NULL;
	int dcrq = reqtoi(req);
	id = ID_MASK(id);
	uint16_t tmp = dcrq << ID_BITS | id;
	tmp = htons(tmp);
	memcpy(h, &tmp, sizeof(tmp));
	return h;
}

char *fill_message(enum reqcode req, uint32_t id, uint16_t nbchat,
		   uint16_t nb, uint8_t datalen, char *data)
{
	char *h = malloc(MAX_HEADER + datalen);
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
	memcpy(h + 6, &datalen, sizeof(datalen));
	if (datalen > 0)
		memcpy(h + MAX_HEADER, data, datalen);
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

char *fill_push_message(uint16_t id, uint16_t nbchat, uint8_t datalen,
			char *data)
{
	return fill_message(PUSH_MESS, id, nbchat, 0, datalen, data);
}

char *fill_ask_messages(uint16_t id, uint16_t nbchat, uint16_t nb)
{
	return fill_message(ASK_MESS, id, nbchat, nb, 0, NULL);
}

char *fill_subscribe(uint16_t id, uint16_t nbchat)
{
	return fill_message(SUBSCRIBE, id, nbchat, 0, 0, NULL);
}

char *fill_push_file(uint16_t id, uint16_t nbchat, uint8_t datalen,
		     char *data)
{
	return fill_message(PUSH_FILE, id, nbchat, 0, datalen, data);
}

char *fill_pull_file(uint16_t id, uint16_t nbchat, uint16_t nb,
		     uint16_t datalen, char *data)
{
	return fill_message(PULL_FILE, id, nbchat, nb, datalen, data);
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
