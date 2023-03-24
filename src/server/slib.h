#ifndef MEGAPHONE_SLIB_H
#define MEGAPHONE_SLIB_H

#include <stdlib.h>


u_int16_t next_id;
struct map *identifiers;

u_int16_t new_id();

int compare_identifiers(void *key1, void *key2);

void free_identifier(void *id);

void free_nickname(void *nickname);
#endif