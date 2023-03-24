#ifndef MEGAPHONE_SLIB_H
#define MEGAPHONE_SLIB_H

struct map *identifiers;

int compare_identifiers(void *key1, void *key2);

void free_identifier(void *id);

void free_nickname(void *nickname);
#endif