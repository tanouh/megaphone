#ifndef MEGAPHONE_REGISTERING_H
#define MEGAPHONE_REGISTERING_H

#include "../map.h"

#include <arpa/inet.h>

uint16_t new_id(uint16_t *next_id);

int cmp_id(void *key1, void *key2);

void free_id(void *id);

void free_name(void *name);

int accept_registering(int sockclient, struct map *identifiers,
		       uint16_t *next_id);

#endif