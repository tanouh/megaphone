#ifndef MEGAPHONE_TEST_CHAT_H
#define MEGAPHONE_TEST_CHAT_H

#include "../../src/ticket.h"
#include "../../src/server/chat.h"

void *test_chat();
void print_chat(struct chat *c);
void print_ticket(struct ticket *t);
#endif