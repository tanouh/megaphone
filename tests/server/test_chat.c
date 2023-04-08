#include "test_chat.h"

#include "../../src/server/chat.h"
#include "../../src/lib.h"
#include "../../src/ticket.h"
#include "../testlib.h"
#include "../printlib.h"
#include "../test_constants.h"
#include <stdlib.h>

static int test_add_ticket_to_chat();
//static int test_get_chat();

// static int cmpc (uint16_t *a, struct chat *b)
// {
// 	if(*a == b->id){
// 		return 0;
// 	}
// 	return (*a < b->id) ? 1 : -1;
// }
//static int test_get_chat();

// static int cmpc (uint16_t *a, struct chat *b)
// {
// 	if(*a == b->id){
// 		return 0;
// 	}
// 	return (*a < b->id) ? 1 : -1;
// }

static int cmptck (struct ticket *t1, struct ticket *t2)
{
	if ((t1->owner != t2->owner) || (t1->feed->id != t2->feed->id) 
	|| (t1->isFile != t2->isFile) || (t1->datalen != t2->datalen)){
		return -1;
	}
	for (int i = 0; i < t1->datalen ; i++){
		if(t2->data[i] != t1->data[i]){
			return -1;
		}
	}
	return 0;
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
	struct chat *c = build_chat();
	struct ticket *t1 = build_ticket(USER,NULL,TEXT_SIZE,TEXT,ISNOTFILE);
	add_tickets_to_chat(c,t1);

	struct ticket *t2 = build_ticket(USER,NULL,TEXT_SIZE,TEXT,ISNOTFILE);
	add_tickets_to_chat(c,t2);
	int ret = 1;
	ret &= ASSERT(c->nbMessages == 2);
	ret &= ASSERT(cmptck((struct ticket *)back(c->messages),t1) == 0);
	ret &= ASSERT(cmptck((struct ticket *)front(c->messages),t2) == 0);
	free(c);
	free(t1);
	free(t2);
	return ret;
}
// static int test_get_chat()
// {
// 	truct chat *c = build_chat();
// 	struct ticket *t1 = build_ticket(USER,NULL,TEXT_SIZE,TEXT,ISNOTFILE);
// 	add_tickets_to_chat(c,t1);
// 	struct ticket *t2 = build_ticket(USER,NULL,TEXT_SIZE,TEXT,ISNOTFILE);
// 	add_tickets_to_chat(c,t2);
// 	int ret =1;


// 	free(c);
// 	free(t1);
// 	free(t2);
// 	return ret;


// }
