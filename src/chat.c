#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#include "ticket.h"
#include "chat.h"

int chat_counter = 0;
struct array *all_chats = NULL;
pthread_mutex_t chatlock = PTHREAD_MUTEX_INITIALIZER;

static int cmp (uint16_t *a, struct chat *b){
	if(*a == b->id){
		return 0;
	}
	return (*a < b->id) ? 1 : -1;
}

void increase_chat_counter(){
	pthread_mutex_lock(&chatlock);
	chat_counter++;
	pthread_mutex_unlock(&chatlock);
}

struct chat *build_chat(){
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
	/*
	TODO : création des tableaux dynamiques des files
	*/
	return c;
}

int add_tickets_to_chat(struct chat *c, struct ticket *t){
	return push_back(c->messages,t);
}
struct chat *get_chat(uint16_t chat_id){
	if(chat_id > chat_counter){
		perror("Chat doesn\'t exist");
		return NULL;
	}
	if (chat_id == 0){
		return build_chat();
	}
	int i = search(all_chats, &chat_id, (int (*)(void *, void *))cmp);
	if (i == -1)
		return NULL;
	else
		return at(all_chats, i);
}
