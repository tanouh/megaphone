#include "ticket.h"
#include "lib.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct ticket *build_ticket(uint16_t owner, struct chat *chat, uint16_t datalen, char *data, int isFile)
{
	struct ticket *t = malloc(sizeof(struct ticket));
	if(t==NULL){
		perror("malloc ticket failed");
		return NULL;
	}
	t->owner = owner;
	t->feed = chat;
	memset(t->data,0,SBUF+1);
	memcpy(t->data,data,datalen);
	memcpy(t->data+datalen, "/0", 1);
	t->datalen = datalen;
	t->isFile = isFile;
	return t;
}
int set_chat(struct ticket *t, struct chat *c)
{
	if(c == NULL){
		perror("Chat identifier is null.");
		return -1;
	}
	t->feed = c;
	return 0;
}
