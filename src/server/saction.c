#include "saction.h"

#include "../array.h"
#include "../lib.h"
#include "../map.h"
#include "../msghead.h"
#include "../ticket.h"
#include "chat.h"
#include "registering.h"
#include "smesslib.h"
#include "askmsg.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// variable globale : liste des users
pthread_mutex_t mpushmess = PTHREAD_MUTEX_INITIALIZER;

void *execute_action(void *arg, int sockclient, struct map *identifiers,
		     uint16_t *next_id, struct array *id_available)
{
	struct msghead h;
	char data[SIZE_MSG];
	char *buf = malloc(SBUF);
	int ret = get_min_header(arg, &h);
	int index = -1;

	if (ret == -1) {
		h = fill_errhead(0);
		fill_message(h, buf, SBUF);
		return buf;
	}
	switch (h.req) {
	case INSCRIPTION:
		if (accept_registering(sockclient, identifiers, next_id,
				       id_available) == -1) {
			h = fill_errhead(0);
			fill_message(h, buf, SBUF);
		} // Reponse serveur ?
		break;
	case PUSH_MESS:
		memset(buf, 0, SBUF);
		ret = get_message(arg, &h, data, SIZE_MSG);

		pthread_mutex_lock(&mpushmess);
		index = push_mess(identifiers, &(h.id), h.chat, h.datalen,
				  data);
		pthread_mutex_unlock(&mpushmess);

		if (index == -1) {
			h = fill_errhead(0);
			fill_message(h, buf, SBUF);
		} else {
			fill_message(h, buf, SBUF);
		}
		break;
	case ASK_MESS: // TODO
		memset(buf, 0, SBUF);
		ret = get_message(arg, &h, data, SIZE_MSG);
		
		index = ask_mess(identifiers, &h, buf, SBUF);
		break;
	case SUBSCRIBE: // TODO
		break;
	case PUSH_FILE: // TODO
		break;
	case PULL_FILE: // TODO
		break;
	default:
		h = fill_errhead(0);
		fill_message(h, buf, SBUF);
		return buf;
	}
	return buf;
}

int push_mess(struct map *identifiers, uint16_t *id, uint16_t chat,
	      uint16_t datalen, void *data)
{
	struct chat *c;
	struct ticket *t;
	uint16_t u;

	if (get_map(identifiers, id, (void *)&u, sizeof(uint16_t)) == -1) {
		perror("User not found");
		return -1;
	}

	if ((c = get_chat(*id, chat)) == NULL) {
		perror("chat not found/couldn't be created"); // GERER
		return -1;
	}
	if ((t = build_ticket(*id, c, datalen, (char *)data, 0)) == NULL) {
		perror("ticket creation failer "); // GERER
		return -1;
	}
	if (add_tickets_to_chat(c, t) == -1) {
		perror("ticket couldn't be added");
		return -1;
	}
	return c->id;
}
