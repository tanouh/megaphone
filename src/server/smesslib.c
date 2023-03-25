#include "smesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../msghead.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fill_message(struct msghead h, char *buf, int bufsize)
{

	if (buf == NULL || bufsize < HEADER_SERVER)
		return -1;
	if (fill_min_header(h, buf, bufsize) == -1) {
		return -1;
	}
	uint16_t be = htons(h.chat);
	memcpy(buf + MIN_HEADER, &be, sizeof(be));
	be = htons(h.nb);
	memcpy(buf + 4, &be, sizeof(be));
	return HEADER_SERVER;
}

int fill_asked_message(struct msghead h, char *buf, int bufsize, void *origin,
		       void *owner, void *data)
{
	int size = 2 * NAMELEN + sizeof(h.chat) + sizeof(h.datalen) + h.datalen;
	if (buf == NULL || bufsize < size)
		return -1;
	uint16_t be = htons(h.chat);
	memcpy(buf, &be, sizeof(be));
	memcpy(buf + sizeof(h.chat), origin, NAMELEN);
	memcpy(buf + sizeof(h.chat) + NAMELEN, owner, NAMELEN);
	memcpy(buf + 2 * NAMELEN + sizeof(h.chat), &(h.datalen),
	       sizeof(h.datalen));
	memcpy(buf + 2 * NAMELEN + sizeof(h.chat) + sizeof(h.datalen), data,
	       h.datalen);
	return size;
}

int fill_subscribe(struct msghead h, char *buf, int bufsize, void *addrmult)
{
	if (bufsize < HEADER_SERVER + ADDRMULT_LEN)
		return -1;
	if (fill_message(h, buf, bufsize) == -1)
		return -1;
	memcpy(buf + HEADER_SERVER, addrmult, ADDRMULT_LEN);
	return HEADER_SERVER + ADDRMULT_LEN;
}

int fill_notification(struct msghead h, char *buf, int bufsize, void *owner,
		      void *data)
{
	if (h.datalen > NOTIFICATION_CONTENT)
		h.datalen = NOTIFICATION_CONTENT;
	int size = HEADER_SERVER + NAMELEN + h.datalen;
	if (bufsize < size)
		return -1;
	if (fill_message(h, buf, bufsize) == -1)
		return -1;
	memcpy(buf + HEADER_SERVER - 2, owner, NAMELEN);
	memset(buf + HEADER_SERVER - 2 + NAMELEN, 0, NOTIFICATION_CONTENT);
	memcpy(buf + HEADER_SERVER - 2 + NAMELEN, data, h.datalen);
	return size;
}

int fill_udp(struct msghead h, char *buf, int bufsize, void *data)
{
	if (bufsize < h.datalen + HEADER_SERVER)
		return -1;
	if (fill_min_header(h, buf, bufsize) == -1)
		return -1;
	uint16_t be = htons(h.nb);
	memcpy(buf + MIN_HEADER, &be, sizeof(be));
	memcpy(buf + MIN_HEADER + sizeof(be), data, h.datalen);
	return h.datalen + MIN_HEADER + sizeof(be);
}

int get_message(const void *msg, struct msghead *h, char *buf, int bufsize)
{
	if (get_min_header(msg, h) == -1)
		return -1;
	msg += MIN_HEADER;
	if (memcpy(&(h->chat), msg, sizeof(h->chat)) == NULL)
		return -1;
	h->chat = ntohs(h->chat);
	msg += sizeof(h->chat);
	if (memcpy(&(h->nb), msg, sizeof(h->nb)) == NULL)
		return -1;
	h->nb = ntohs(h->nb);
	printf("h->nb = %u\n", h->nb);

	msg += sizeof(h->nb);
	if (memcpy(&(h->datalen), msg, sizeof(h->datalen)) == NULL)
		return -1;
	msg += sizeof(h->datalen);
	if (buf != NULL && bufsize >= h->datalen) {
		if (memcpy(buf, msg, h->datalen) == NULL)
			return -1;
	}
	return MIN_HEADER + sizeof(h->chat) + sizeof(h->nb) + h->datalen;
}

int get_inscription(void *msg, struct msghead *h, char *buf)
{
	if (get_min_header(msg, h) == -1)
		return -1;
	memcpy(buf, msg + MIN_HEADER, NAMELEN);
	return MIN_HEADER + NAMELEN;
}

int get_udp(const char *msg, uint16_t msglen, struct msghead *h, char *buf,
	    int bufsize)
{
	if (get_min_header(msg, h) == -1)
		return -1;
	msg += MIN_HEADER;
	if (memcpy(&(h->nb), msg, sizeof(h->nb)) == NULL)
		return -1;
	h->nb = ntohs(h->nb);
	msg += sizeof(h->nb);
	h->datalen = msglen - sizeof(h->nb) - MIN_HEADER;
	if (bufsize < h->datalen)
		return -1;
	if (memcpy(buf, msg, h->datalen) == NULL)
		return -1;
	return MIN_HEADER + sizeof(h->nb) + h->datalen;
}
