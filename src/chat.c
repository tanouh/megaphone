#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "ticket.h"
#include "chat.h"
#include "user.h"


pthread_mutex_t msrv = PTHREAD_MUTEX_INITIALIZER;

struct chat *build_chat(){
	struct chat *c = malloc(sizeof(struct chat));
	if(c == NULL){
		perror("malloc chat failed");
		return NULL;
	}
	pthread_mutex_lock(&msrv);
	chat_counter++;
	pthread_mutex_unlock(&msrv);
	c->id = chat_counter;
	c->nbMessages = 0;
	/*
	TODO : création des tableaux dynamiques 
	*/
	return c;
}

void add_tickets_to_chat(int chat_id, struct ticket *t){

}
struct chat *find_chat(int chat_id){
	return NULL;
}
void print_chat(int chat_id, int nbMsg){
	
}