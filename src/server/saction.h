#ifndef MEGAPHONE_SACTION_H
#define MEGAPHONE_SACTION_H

#include <arpa/inet.h>
void *execute_action(void *arg);
int push_mess(uint16_t id, uint16_t chat, uint16_t datalen, void *data);
#endif