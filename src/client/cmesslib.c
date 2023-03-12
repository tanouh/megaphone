#include "cmesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *fill_message(enum reqcode req, uint16_t id, uint16_t chat, uint16_t nb,
		   uint8_t datalen, const void *data, int *size_msg)
{
	void *h = malloc(MAX_HEADER + datalen);
	if (h == NULL)
		return NULL;
	void *tmp = fill_min_header(req, id);
	if (tmp == NULL) {
		free(h);
		return NULL;
	}
	memcpy(h, tmp, MIN_HEADER);
	free(tmp);
	uint16_t be = htons(chat);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	be = htons(nb);
	memcpy(h + 4, &be, sizeof(be));
	memcpy(h + 6, &datalen, sizeof(datalen));
	if (datalen > 0)
		memcpy(h + MAX_HEADER, data, datalen);
	if (size_msg != NULL)
		*size_msg = MAX_HEADER + datalen;
	return h;
}

void *fill_inscription(const void *nickname, int len, int *size_msg)
{
	void *h = fill_min_header(INSCRIPTION, 0);
	if (h == NULL)
		return NULL;
	if ((h = realloc(h, MIN_HEADER + NAMELEN)) == NULL) {
		free(h);
		return NULL;
	}
	memset(h + MIN_HEADER, '#', NAMELEN);
	memcpy(h + MIN_HEADER, nickname, len);
	if (size_msg != NULL)
		*size_msg = MIN_HEADER + NAMELEN;
	return h;
}

void *fill_push_message(uint16_t id, uint16_t chat, uint8_t datalen,
			const void *data, int *size_msg)
{
	return fill_message(PUSH_MESS, id, chat, 0, datalen, data, size_msg);
}

void *fill_ask_messages(uint16_t id, uint16_t chat, uint16_t nb, int *size_msg)
{
	return fill_message(ASK_MESS, id, chat, nb, 0, NULL, size_msg);
}

void *fill_subscribe(uint16_t id, uint16_t chat, int *size_msg)
{
	return fill_message(SUBSCRIBE, id, chat, 0, 0, NULL, size_msg);
}

void *fill_push_file(uint16_t id, uint16_t chat, uint8_t datalen,
		     const void *data, int *size_msg)
{
	return fill_message(PUSH_FILE, id, chat, 0, datalen, data, size_msg);
}

void *fill_pull_file(uint16_t id, uint16_t chat, uint16_t nb, uint16_t datalen,
		     const void *data, int *size_msg)
{
	return fill_message(PULL_FILE, id, chat, nb, datalen, data, size_msg);
}

void *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       const void *data, int *size_msg)
{
	void *h = fill_min_header(req, id);
	if ((h = realloc(h, MIN_HEADER + sizeof(nb) + datalen)) == NULL) {
		return NULL;
	}
	uint16_t be = htons(nb);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	memcpy(h + MIN_HEADER + sizeof(be), data, datalen);
	if (size_msg != NULL)
		*size_msg = MIN_HEADER + sizeof(nb) + datalen;
	return h;
}

void *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen,
			 const void *data, int *size_msg)
{
	return fill_udp(PUSH_FILE, id, nb, datalen, data, size_msg);
}

int get_message(const void *msg, enum reqcode *req, uint16_t *id,
		uint16_t *chat, uint16_t *nb)
{
	uint16_t buf;
	int s = sizeof(buf);
	if (get_min_header(msg, req, id) != 0)
		return -1;
	msg += MIN_HEADER;
	if (memcpy(&buf, msg, s) == NULL)
		return -1;
	if (chat != NULL)
		*chat = ntohs(buf);
	msg += s;
	if (memcpy(&buf, msg, s) == NULL)
		return -1;

	if (nb != NULL) {
		*nb = ntohs(buf);
	}
	if (*req == ERROR)
		return 1;
	else
		return 0;
}

int get_asked_messages(const void *msg, uint16_t *chat, void **origin,
		       void **owner, uint8_t *datalen, void **data)
{
	uint16_t buf;
	int s = sizeof(buf);
	memcpy(&buf, msg, s);
	if (chat != NULL)
		*chat = ntohs(buf);
	msg += s;
	if (origin != NULL && fill_buffer(msg, origin, NAMELEN) == -1)
		goto error_free;

	msg += NAMELEN;

	if (owner != NULL && fill_buffer(msg, owner, NAMELEN) == -1)
		goto error_free;
	msg += NAMELEN;
	if (datalen == NULL)
		return 0;
	s = sizeof(*datalen);

	memcpy(datalen, msg, s);
	msg += s;
	if (data != NULL && fill_buffer(msg, data, *datalen) == -1)
		goto error_free;
	return 0;

error_free:
	if (origin != NULL && *origin != NULL) {
		free(*origin);
		*origin = NULL;
	}

	if (owner != NULL && *owner != NULL) {
		free(*owner);
		*owner = NULL;
	}
	if (data != NULL && *data != NULL) {
		free(*data);
		*data = NULL;
	}
	return -1;
}

int get_subscribed_message(const void *msg, enum reqcode *req, uint16_t *id,
			   uint16_t *chat, uint16_t *nb, void **addr)
{
	int err;
	if ((err = get_message(msg, req, id, chat, nb)) != 0)
		return err;
	msg += HEADER_SERVER;
	if (addr != NULL && fill_buffer(msg, addr, ADDRMULT_LEN) == -1)
		return -1;
	return 0;
}

int get_notification_message(const void *msg, enum reqcode *req, uint16_t *id,
			     uint16_t *chat, void **owner, void **data)
{
	int err;
	if ((err = get_message(msg, req, id, chat, NULL) != 0))
		return err;
	msg += HEADER_SERVER - 2;
	if (owner != NULL && fill_buffer(msg, owner, NAMELEN) == -1)
		return -1;
	msg += NAMELEN;
	if (data != NULL &&
	    fill_buffer(msg, data, NOTIFICATION_CONTENT) == -1) {
		if (owner != NULL && *owner != NULL)
			free(*owner);
		return -1;
	}

	return 0;
}

int get_udp_message(const void *msg, int msglen, enum reqcode *req,
		    uint16_t *id, uint16_t *block, uint16_t *datalen,
		    void **data)
{
	int err;
	if ((err = get_message(msg, req, id, block, NULL)) != 0)
		return err;
	msg += HEADER_SERVER - 2;
	msglen -= HEADER_SERVER - 2;
	if (msglen <= 0) {
		if (data != NULL)
			*data = NULL;
		if (datalen != NULL)
			*datalen = 0;
		return 0;
	}
	if (datalen == NULL || data == NULL)
		return -1;
	*datalen = msglen;
	if (fill_buffer(msg, data, msglen) == -1)
		return -1;
	return 0;
}
