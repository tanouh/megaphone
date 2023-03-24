#ifndef MEGAPHONE_SACTION_H
#define MEGAPHONE_SACTION_H

#include "../map.h"

#include <arpa/inet.h>
void *execute_action(void *arg, int sockclient, struct map *ids, uint16_t *next_id);
int push_mess(struct map *ids, uint16_t *id, uint16_t chat, uint16_t datalen, void *data);
#endif