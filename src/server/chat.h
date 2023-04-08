#ifndef MEGAPHONE_CHAT_H
#define MEGAPHONE_CHAT_H

#include "../map.h"

#include <arpa/inet.h>

struct chat{
	uint16_t id; 
	size_t nbMessages;
	struct array *messages;
	struct array *followers;
};
extern int chat_counter; 
extern struct map *all_chats; /* A changer en hashmap */

struct chat *build_chat();
/**
* @brief Add an element in a chat
* @returns 0 on succes and -1 on failure  
*/
int add_tickets_to_chat(struct chat *c, void *t);

/**
* @brief gets the chat in all_chats map
* @param chat_id used as the key in the hashmap
* @returns the adress of the chat if found and -1 otherwise
*/
struct chat *get_chat(uint16_t chat_id);
/**
* @brief Compares two chat id as key of map
* @returns 0 if identical -1 otherwise
*/
int cmp_ckey(uint16_t *key1, uint16_t *key2);

/* get n_tickets_in_chat() retourne un tableau dynamique de tickets (?) */ 

//fonction suppress_chat()
#endif