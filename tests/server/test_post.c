#include "test_post.h"
#include "test_chat.h"
#include "../test_constants.h"
#include "../printlib.h"
#include "../testlib.h"
#include "../../src/map.h"
#include "../../src/ticket.h"
#include "../../src/server/chat.h"
#include "../../src/server/saction.h"
#include "../../src/server/registering.h"
#include "../../src/lib.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int test_push_mess();

void * test_post(){
	int ret = 1;
	ret &= test_c(test_push_mess, "test_push_mess", print_meg);
	return malloc_return(ret);	
}
static int test_push_mess(){
	int ret =1;
	struct map *ids = make_map((int (*)(void *, void *))cmp_ckey, int_hash);
	struct map *chats = make_map((int (*)(void *, void *))cmp_ckey, int_hash);

	uint16_t user = USER;
	

	ret &= (put_map(ids, &user, NAME, NULL, sizeof(uint16_t), 
						strlen(NAME)) != -1);
	
	struct chat *c = build_chat(chats, user);
	ret &= (push_mess(chats, ids, &user, &(c->id), TEXT_SIZE, TEXT) != -1);
	struct ticket *tc = back(c->tickets);
	uint16_t tmpid = 1;
	struct chat *tmp = get_chat(chats, user, &tmpid);
	ret &= (tmp != NULL);
	struct ticket *ttmp = back(tmp->tickets);
	ret &= (ttmp != NULL);
	ret &= (strcmp(tc->data, ttmp->data) != 0);
	free_chat(tmp);
	free_map(chats, NULL, free_chat);
	free_map(ids, NULL, NULL);
	return ret;
}