#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#include "chat.h"
#include "../ticket.h"

int chat_counter = 0;
struct map *all_chats = NULL;
pthread_mutex_t mchat = PTHREAD_MUTEX_INITIALIZER;


int cmp_ckey(uint16_t *a, uint16_t *b)
{
	if (a == NULL)
		return (b == NULL) ? 0 : -1;
	if (b == NULL)
		return (a == NULL) ? 0 : -1;

	if (*a == *b)
		return 0;
	return -1;
}
int init_allchats()
{
	all_chats = make_map((int(*)(void *, void *))cmp_ckey, default_hash); // à vérifier
	return (all_chats == NULL) ? -1 : 0;
}

void increase_chat_counter()
{
	pthread_mutex_lock(&mchat);
	chat_counter++;
	pthread_mutex_unlock(&mchat);
}

struct chat *build_chat()
{
	struct chat *c = malloc(sizeof(struct chat));
	if(c == NULL){
		perror("malloc chat failed");
		return NULL;
	}

	increase_chat_counter();
	
	c->id = chat_counter;
	c->nbMessages = 0;
	c->messages = make_array(sizeof(struct ticket)) ;
	c->followers = make_array(sizeof(uint16_t));
	return c;
}

int add_tickets_to_chat(struct chat *c, void *t)
{
	if (push_back(c->messages,t) == -1 || set_chat((struct ticket *)t, c) == -1){
		perror("Couldn't add the ticket in the chat.");
		return -1;
	}
	c->nbMessages ++;
	return 0;

}
struct chat *get_chat(uint16_t chat_id)
{
	struct chat *c = malloc(sizeof(struct chat));
	if(c == NULL){
		perror("malloc failed");
		return NULL;
	}
	if(chat_id > chat_counter){
		perror("Chat doesn't exist");
		return NULL;
	}
	if (chat_id == 0){
		return build_chat();
	}
	int i = get_map(all_chats, (void *)&chat_id, c, sizeof(uint16_t));
	if (i == -1)
		return NULL;
	else
		return c;
}
