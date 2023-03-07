#include "cmesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fill_message(enum reqcode req, uint16_t id, uint16_t chat, uint16_t nb,
		   uint8_t datalen, const char *data)
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
	uint16_t be = htons(chat);
	memcpy(h + MIN_HEADER, &be, sizeof(be));
	be = htons(nb);
	memcpy(h + 4, &be, sizeof(be));
	memcpy(h + 6, &datalen, sizeof(datalen));
	if (datalen > 0)
		memcpy(h + MAX_HEADER, data, datalen);
	return h;
}

char *fill_inscription(const char *nickname, int len)
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

char *fill_push_message(uint16_t id, uint16_t chat, uint8_t datalen,
			const char *data)
{
	return fill_message(PUSH_MESS, id, chat, 0, datalen, data);
}

char *fill_ask_messages(uint16_t id, uint16_t chat, uint16_t nb)
{
	return fill_message(ASK_MESS, id, chat, nb, 0, NULL);
}

char *fill_subscribe(uint16_t id, uint16_t chat)
{
	return fill_message(SUBSCRIBE, id, chat, 0, 0, NULL);
}

char *fill_push_file(uint16_t id, uint16_t chat, uint8_t datalen,
		     const char *data)
{
	return fill_message(PUSH_FILE, id, chat, 0, datalen, data);
}

char *fill_pull_file(uint16_t id, uint16_t chat, uint16_t nb, uint16_t datalen,
		     const char *data)
{
	return fill_message(PULL_FILE, id, chat, nb, datalen, data);
}

char *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       const char *data)
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

char *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen,
			 const char *data)
{
	return fill_udp(PUSH_FILE, id, nb, datalen, data);
}

int get_message(const char *msg, enum reqcode *req, uint16_t *id,
		uint16_t *chat, uint16_t *nb)
{
	uint16_t buf;
	int s = sizeof(buf);
	if (memcpy(&buf, msg, s) == NULL)
		return -1;
	msg += s;
	buf = ntohs(buf);
	if (req != NULL)
		*req = itoreq(REQ_MASK(buf));
	if (id != NULL)
		*id = ID_MASK(buf);
	if (memcpy(&buf, msg, s) == NULL)
		return -1;
	if (chat != NULL)
		*chat = ntohs(buf);
	msg += s;
	if (memcpy(&buf, msg, s) == NULL)
		return -1;
	if (nb != NULL)
		*nb = ntohs(buf);
	if (*req == ERROR)
		return 1;
	else
		return 0;
}

int get_asked_messages(const char *msg, uint16_t *chat, char **origin,
		       char **owner, uint8_t *datalen, char **data)
{
	uint16_t buf;
	int s = sizeof(buf);
	memcpy(&buf, msg, s);
	if (chat != NULL)
		*chat = ntohs(buf);
	msg += s;
	if (origin != NULL && fill_buffer(msg, origin, NAMELEN) == -1)
		goto error_free;
	msg += s;
	if (owner != NULL && fill_buffer(msg, owner, NAMELEN) == -1)
		goto error_free;
	if (datalen == NULL)
		return 0;
	msg += s;
	s = sizeof(datalen);
	memcpy(datalen, msg, s);
	msg += s;
	if (data != NULL && fill_buffer(msg, data, (int)*datalen) == -1)
		goto error_free;
	return 0;

error_free:
	if (origin != NULL && *origin != NULL)
		free(*origin);
	if (owner != NULL && *owner != NULL)
		free(*owner);
	if (data != NULL && *data != NULL)
		free(*data);
	return -1;
}

int get_subscribed_message(const char *msg, enum reqcode *req, uint16_t *id,
			   uint16_t *chat, uint16_t *nb, char **addr)
{
	int err;
	if ((err = get_message(msg, req, id, chat, nb)) != 0)
		return err;
	msg += HEADER_SERVER;
	if (addr != NULL && fill_buffer(msg, addr, ADDRMULT_LEN) == -1)
		return -1;
	return 0;
}

int get_notification_message(const char *msg, enum reqcode *req, uint16_t *id,
			     uint16_t *chat, char **owner, char **data)
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
