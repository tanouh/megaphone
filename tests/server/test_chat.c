#include "test_chat.h"

#include "../../src/server/chat.h"
#include "../../src/server/registering.h"
#include "../../src/lib.h"
#include "../../src/ticket.h"
#include "../testlib.h"
#include "../printlib.h"
#include "../test_constants.h"
#include <stdlib.h>
#include <stdio.h>

static int test_add_ticket_to_chat();
//static int test_get_chat();


static int cmptck (struct ticket *t1, struct ticket *t2)
{
	if ((t1->owner != t2->owner) || (t1->chat != t2->chat) 
	|| (t1->isFile != t2->isFile) || (t1->datalen != t2->datalen)){
		return 0;
	}
	for (int i = 0; i < t1->datalen ; i++){
		if(t2->data[i] != t1->data[i]){
			return 0;
		}
	}
	return 1;
}

void *test_chat()
{
	int ret = 1;
	ret &= test_c(test_add_ticket_to_chat, "test_add_ticket_to_chat", 
			print_meg);
	// ret &= test_c(test_get_chat, "test_get_chat", print_meg);
	return malloc_return(ret);
}

static int test_add_ticket_to_chat()
{
	struct map *chats = make_map(cmp_id, default_hash);
	struct chat *c = build_chat(chats, USER);
	struct ticket *t1 = build_ticket(USER,TEXT_SIZE,TEXT,ISNOTFILE);
	add_tickets_to_chat(c,t1);

	struct ticket *t2 = build_ticket(USER,TEXT_SIZE,TEXT,ISNOTFILE);
	add_tickets_to_chat(c,t2);
	int ret = 1;
	ret &= ASSERT(get_ntickets(c) == 2);
	ret &= ASSERT(cmptck((struct ticket *)back(c->tickets),t1) == 0);
	ret &= ASSERT(cmptck((struct ticket *)front(c->tickets),t2) == 0);
	free(t1);
	free(t2);
	free_chat(c);
	return ret;
}
void print_chat(struct chat *c){
	printf("Chat n. : %d\nInitUser : %d\nNbMsg: %ld\n", c->id, c->origin_user, get_ntickets(c));
}
void print_ticket(struct ticket *t){
	printf("Ticket n. : %d\nNchat: %d\n", t->owner, t->chat);
}
