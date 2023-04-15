#ifndef MEGAPHONE_CHAT_H
#define MEGAPHONE_CHAT_H

#include "../map.h"

#include <arpa/inet.h>

struct chat{
	uint16_t id; 
	uint16_t origin_user;
	struct array *tickets;
};
extern int chat_counter; 
extern struct map *all_chats; /* A changer en hashmap */

/**
* @brief Create a new chat 
* @param initialiser the user who created the chat
*/ 
struct chat *build_chat(struct map *chats, uint16_t initialiser);
/**
* @brief Add an element in a chat
* @returns 0 on succes and -1 on failure  
*/
int add_tickets_to_chat(struct chat *c, void *t);

/**
* @brief gets the chat in all_chats map
* @param all_chat chats map
* @param user the request maker
* @param chat_id used as the key in the hashmap
* @returns the adress of the chat if found and -1 otherwise
*/
struct chat *get_chat(struct map *all_chats, uint16_t user, uint16_t *chat_id);
/**
* @brief Compares two chat id as key of map
* @returns 0 if identical -1 otherwise
*/
int cmp_ckey(uint16_t *key1, uint16_t *key2);
/**
 * @brief Get the current nb of tickets in a chat
 * 
 * @param c 
 * @return size_t 
 */
size_t get_ntickets(struct chat *c);
/**
 * @brief 
 * 
 * @param c 
 * @param free_elem 
 */
void free_chat(void *c);

#endif