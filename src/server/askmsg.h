#ifndef MEGAPHONE_ASKMSG_H
#define MEGAPHONE_ASKMSG_H

#include <arpa/inet.h>
#include "../msghead.h"
#include "../map.h"


/**
 * @brief Get last messages from a/all chat(s)
 * 
 * @param identifiers list of all active users
 * @param h 
 * @param buf 
 * @param sizebuf 
 * @return the nb of tickets sent 
 */
int ask_mess (struct map *identifiers, struct msghead *h, 
	void *buf, int sizebuf);

/**
 * @brief Get the last n messages 
 * 
 * @param chat id of the chat
 * @param n nb of messages requested
 * @param buf 
 * @return nb of msg put in buf 
 */
// int get_last_n_messages(struct map *identifiers, struct msghead *h, struct chat *c,
// 	void *buf, int sizebuf);

#endif