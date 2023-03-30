#ifndef MEGAPHONE_CHAT_H
#define MEGAPHONE_CHAT_H

#include "../array.h"
#include "../ticket.h"

#include <arpa/inet.h>

struct chat{
	uint16_t id; 
	size_t nbMessages;
	struct array *messages;
	struct array *followers;
	/* ? : pour les fichiers c'est comment ?*/
};
extern int chat_counter; 
extern struct array *all_chats; /* A changer en hashmap */

struct chat *build_chat();
int add_tickets_to_chat(struct chat *c, struct ticket *t);
struct chat *get_chat(uint16_t chat_id);
/* get n_tickets_in_chat() retourne un tableau dynamique de tickets (?) */ 

#endif