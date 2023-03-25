#ifndef MEGAPHONE_MSGHEAD_H
#define MEGAPHONE_MSGHEAD_H
#include "reqcode.h"

#include <arpa/inet.h>

struct msghead {
	enum reqcode req;
	uint16_t id;
	uint16_t chat;
	uint16_t nb;
	uint8_t datalen;
};

struct msghead fill_msghead(enum reqcode req, uint16_t id, uint16_t chat,
			    uint16_t nb, uint8_t datalen);

#endif
