#ifndef MEGAPHONE_TICKET_H
#define MEGAPHONE_TICKET_H

#include "constants.h"
#include <arpa/inet.h>

struct ticket {
	uint16_t owner;
	struct chat *feed;
	char data[SBUF];
	int isFile; /*0 false 1 true*/
};

struct ticket *build_ticket(uint16_t owner, struct chat *c, uint16_t datalen, char *data, int isFile);

#endif