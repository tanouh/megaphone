#ifndef MEGAPHONE_REGISTERING_H
#define MEGAPHONE_REGISTERING_H

#include "../map.h"

#include <arpa/inet.h>

uint16_t new_id(uint16_t *next_id);

int compare_identifiers(void *key1, void *key2);

void free_identifier(void *id);

void free_nickname(void *nickname);

int accept_registering(int sockclient, struct map *identifiers,
		       uint16_t *next_id);

#endif