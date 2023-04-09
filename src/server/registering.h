#ifndef MEGAPHONE_REGISTERING_H
#define MEGAPHONE_REGISTERING_H

#include "../map.h"
#include "../array.h"

#include <arpa/inet.h>

/**
 * @brief Gives an available id
 * @param next_id The requested id
 * @param id_available An array containing the id of unsubscribed users
 * @return An available id
 */
uint16_t new_id(uint16_t *next_id, struct array *id_available);



/**
 * @brief Frees @param id
 */
void free_id(void *id);

/**
 * @brief Frees @param name
 */
void free_name(void *name);

/**
 * @brief Accepts the inscription of a client
 * @param sockclient The socket linking the server to the client
 * @param identifiers The hashmap that contains all the registered users
 * @param next_id The id that the user will receive
 * @return 0 if the inscription was accepted, -1 otherwise
 */
int accept_registering(int sockclient, struct map *identifiers,
		       uint16_t *next_id, struct array *id_available);

#endif
