#include "cmesslib.h"

#include "../constants.h"
#include "../lib.h"
#include "../msghead.h"
#include "../reqcode.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int fill_message(struct msghead h, char *buf, size_t bufsize, const void *data)
{
	if (bufsize < MAX_HEADER + (size_t)h.datalen)
		return -1;
	if (fill_min_header(h, buf, bufsize) == -1)
		return -1;
	uint16_t be = htons(h.chat);
	memcpy(buf + MIN_HEADER, &be, sizeof(be));
	be = htons(h.nb);
	memcpy(buf + 4, &be, sizeof(be));
	memcpy(buf + 6, &(h.datalen), sizeof(h.datalen));
	if (h.datalen > 0)
		memcpy(buf + MAX_HEADER, data, h.datalen);
	return MAX_HEADER + h.datalen;
}

int fill_inscription(struct msghead h, char *buf, size_t bufsize,
		     const char *nickname, int len)
{
	if (bufsize < MIN_HEADER + NAMELEN)
		return -1;
	if (fill_min_header(h, buf, bufsize) == -1)
		return -1;
	memset(buf + MIN_HEADER, '#', NAMELEN);
	memcpy(buf + MIN_HEADER, nickname, len);
	return MIN_HEADER + NAMELEN;
}

int fill_udp(struct msghead h, char *buf, size_t bufsize, const void *data)
{
	if (bufsize < MIN_HEADER + sizeof(h.nb) + h.datalen)
		return -1;
	if (fill_min_header(h, buf, bufsize) == -1)
		return -1;
	uint16_t be = htons(h.nb);
	memcpy(buf + MIN_HEADER, &be, sizeof(be));
	memcpy(buf + MIN_HEADER + sizeof(be), data, h.datalen);
	return MIN_HEADER + sizeof(h.nb) + h.datalen;
}

int get_message(const void *msg, struct msghead *h)
{
	uint16_t buf;
	int s = sizeof(buf);
	if (get_min_header(msg, h) == -1)
		return -1;
	msg += MIN_HEADER;
	if (memcpy(&buf, msg, s) == NULL)
		return -1;
	h->chat = ntohs(buf);
	msg += s;
	if (memcpy(&buf, msg, s) == NULL)
		return -1;

	h->nb = ntohs(buf);
	return MAX_HEADER;
}

int get_asked_messages(const void *msg, struct msghead *h, void *origin,
		       void *owner, void *buf, size_t bufsize)
{
	uint16_t tmp;
	int s = sizeof(tmp);
	memcpy(&tmp, msg, s);
	h->chat = ntohs(tmp);
	msg += s;
	if (origin != NULL)
		memcpy(origin, msg, NAMELEN);
	msg += NAMELEN;
	if (owner != NULL)
		memcpy(owner, msg, NAMELEN);
	msg += NAMELEN;
	memcpy(&(h->datalen), msg, sizeof(h->datalen));
	msg += sizeof(h->datalen);
	if (buf == NULL || bufsize < h->datalen)
		return -1;
	memcpy(buf, msg, h->datalen);
	return sizeof(tmp) + 2 * NAMELEN + sizeof(h->datalen) + h->datalen;
}

int get_subscribed_message(const void *msg, struct msghead *h, void *addr)
{
	int size = 0;
	size = get_message(msg, h);
	if (size == -1)
		return -1;
	msg += HEADER_SERVER;
	if (addr == NULL)
		return -1;
	memcpy(addr, msg, ADDRMULT_LEN);
	return size + ADDRMULT_LEN;
}

int get_notification_message(const void *msg, struct msghead *h, void *owner,
			     void *data)
{
	if ((get_message(msg, h) == -1))
		return -1;
	msg += HEADER_SERVER - 2;
	if (owner != NULL)
		memcpy(owner, msg, NAMELEN);
	msg += NAMELEN;
	if (data != NULL)
		memcpy(data, msg, NOTIFICATION_CONTENT);
	return MIN_HEADER + NAMELEN + sizeof(h->chat) + NOTIFICATION_CONTENT;
}

int get_udp_message(const void *msg, size_t msglen, struct msghead *h,
		    void *buf, size_t bufsize)
{
	if (get_min_header(msg, h) == -1)
		return -1;
	msg += MIN_HEADER;
	memcpy(&(h->nb), msg, sizeof(h->nb));
	h->nb = ntohs(h->nb);
	msg += sizeof(h->nb);
	msglen -= MIN_HEADER + 2;
	if (msglen <= 0) {
		h->datalen = 0;
		return 0;
	}
	if (buf == NULL)
		return -1;
	h->datalen = msglen;
	int rd = (bufsize < h->datalen) ? bufsize : h->datalen;
	memcpy(buf, msg, rd);
	return MIN_HEADER + 2 + rd;
}
