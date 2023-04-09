#ifndef MEGAPHONE_CHAT_H
#define MEGAPHONE_CHAT_H

#include "../map.h"

#include <arpa/inet.h>

struct chat{
	uint16_t id;
	uint16_t origin_user;
	size_t nbMessages;
	struct array *messages;
	char* addrmult;
	size_t followers;
	size_t last_message_notified;
};
extern int chat_counter;
extern struct map *all_chats;

/**
* @brief Create a new chat
* @param initialiser the user who created the chat
*/
struct chat *build_chat(uint16_t initialiser);
/**
* @brief Add an element in a chat
* @returns 0 on succes and -1 on failure
*/
int add_tickets_to_chat(struct chat *c, void *t);

/**
* @brief gets the chat in all_chats map
* @param user the request maker
* @param chat_id used as the key in the hashmap
* @returns the adress of the chat if found and -1 otherwise
*/
struct chat *get_chat(uint16_t user, uint16_t chat_id);
/**
* @brief Compares two chat id as key of map
* @returns 0 if identical -1 otherwise
*/
int cmp_ckey(uint16_t *key1, uint16_t *key2);

void free_chat(struct chat *chat);

void destruct_chat(struct chat *);




//fonction suppress_chat()
#endif
