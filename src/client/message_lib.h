#ifndef MEGAPHONE_MESSAGE_LIB_H
#define MEGAPHONE_MESSAGE_LIB_H

#include "../codereq.h"

#include <arpa/inet.h>
char *fill_min_header(enum codereq crq, uint32_t id);
char *fill_message(enum codereq crq, uint32_t id, int numfil, int nb,
		   uint32_t datalen, char *data);
char *fill_inscription(char *nickname, int len);

#endif
