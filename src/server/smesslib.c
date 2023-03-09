#include "smesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *fill_message(enum reqcode req, uint32_t id, uint16_t nbchat, uint16_t nb)
{
	void *h = malloc(MAX_HEADER);
	if (h == NULL)
		return NULL;
	void *tmp = fill_min_header(req, id);
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

void *fill_inscription(uint16_t id)
{
	return fill_message(INSCRIPTION, id, 0, 0);
}

void *fill_push_message(uint16_t id, uint16_t nbchat)
{
	return fill_message(PUSH_MESS, id, nbchat, 0);
}

void *fill_ask_messages(uint16_t id, uint16_t nbchat, uint16_t nb)
{
	return fill_message(ASK_MESS, id, nbchat, nb);
}

void *fill_asked_message(uint16_t nbchat, void *origin, void *owner,
			 uint8_t datalen, void *data)
{
	int size = 2 * NAMELEN + sizeof(nbchat) + sizeof(datalen) + datalen;
	void *m = malloc(size);
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

void *fill_subscribe(uint16_t id, uint16_t nbchat, uint16_t nb, void *addrmult)
{
	void *m = fill_message(SUBSCRIBE, id, nbchat, nb);
	if (realloc(m, HEADER_SERVER + ADDRMULT_LEN) == NULL) {
		free(m);
		return NULL;
	}
	memcpy(m + HEADER_SERVER, addrmult, ADDRMULT_LEN);
	return m;
}

void *fill_notification(uint16_t nbchat, void *owner, void *data, int datalen)
{
	if (datalen > NOTIFICATION_CONTENT)
		datalen = NOTIFICATION_CONTENT;
	void *m = fill_message(SUBSCRIBE, 0, nbchat, 0);
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

void *fill_push_file(uint16_t id, uint16_t nbchat)
{
	return fill_message(PUSH_FILE, id, nbchat, 0);
}

void *fill_pull_file(uint16_t id, uint16_t nbchat, uint16_t nb)
{
	return fill_message(PULL_FILE, id, nbchat, nb);
}

void *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       void *data)
{
	void *h = fill_min_header(req, id);
	if (realloc(h, MIN_HEADER + sizeof(nb) + datalen) == NULL) {
		return NULL;
	}
	uint16_t be = htons(nb);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	memcpy(h + MIN_HEADER + sizeof(be), data, datalen);
	return h;
}

void *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen, void *data)
{
	return fill_udp(PUSH_FILE, id, nb, datalen, data);
}

void *fill_error()
{
	return fill_message(ERROR, 0, 0, 0);
}

int get_message(const void *msg, enum reqcode *req, uint16_t *id,
		  uint16_t *chat, uint16_t *nb, uint8_t *datalen, void **data)
{
	if (get_min_header(msg, req, id) != 0)
		return -1;
	msg += MIN_HEADER;
	if (chat != NULL) {
		if (memcpy(chat, msg, sizeof(*chat)) == NULL)
			return -1;
	}
	msg += sizeof(*chat);
	if(nb != NULL) {
		if (memcpy(nb, msg, sizeof(*nb)) == NULL)
			return -1;
	}
	msg += sizeof(*nb);
	if (nb == NULL || data == NULL)
		return -1;
	if (memcpy(datalen, msg, sizeof(*datalen)) == NULL)
		return -1;
	msg += sizeof(*datalen);
	if (fill_buffer(msg, data, *datalen) != 0)
		return -1;
	return 0;
}

int get_udp(const void *msg, uint16_t msglen, enum reqcode *req, uint16_t *id, uint16_t *block, uint16_t *datalen, void **data) {
	if (get_min_header(msg, req, id) != 0)
		return -1;
	msg += MIN_HEADER;
	if(block != NULL) {
		if (memcpy(block, msg, sizeof(*block)) == NULL)
			return -1;
	}
	msg += sizeof(*block);
	if (datalen == NULL || data == NULL)
		return -1;
	*datalen = msglen - sizeof(*block) - MIN_HEADER;
	if (fill_buffer(msg, data, *datalen) != 0)
		return -1;
	return 0;
}
