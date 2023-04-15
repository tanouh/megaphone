#include "chat.h"

#include "../ticket.h"
#include "../map.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

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
	all_chats = make_map((int (*)(void *, void *))cmp_ckey,
			     default_hash); // à vérifier
	return (all_chats == NULL) ? -1 : 0;
}

void increase_chat_counter()
{ //todo : changer suivant le même principe que le ntickets dans chat
	pthread_mutex_lock(&mchat);
	chat_counter++;
	pthread_mutex_unlock(&mchat);
}

struct chat *build_chat(struct map *all_chats, uint16_t u)
{
	struct chat *c = malloc(sizeof(struct chat));
	if (c == NULL) {
		perror("malloc chat failed");
		return NULL;
	}
	pthread_mutex_lock(&mchat);
	c->id = all_chats->nkey + 1;
	c->origin_user = u;
	c->tickets = make_array(sizeof(struct ticket));
	if (put_map(all_chats, &(c->id), c, NULL, sizeof(uint16_t), 
		sizeof(struct chat *)) == -1){
		free_array(c->tickets, NULL);
		free(c);
		perror("Couldn't put the created chat to chats map");
		return NULL;
	}
	struct chat *data = malloc(sizeof(struct chat));
	map_foreach_key(all_chats, data){
		printf("pointer of tickets: %p\n", data->tickets);
	}
	pthread_mutex_unlock(&mchat);
	return c;
}

int add_tickets_to_chat(struct chat *c, void *t)
{
	if (c->tickets == NULL){
		perror("c->tickets null");
		return -1;
	}
	if (push_back(c->tickets, t) == -1 ||
		set_chat((struct ticket *)t, c) == -1) {
		perror("Couldn't add the ticket in the chat.");
		return -1;
	}
	return 0;
}
struct chat *get_chat(struct map *all_chats, uint16_t u, uint16_t *chat_id)
{
	struct chat *c = malloc(sizeof(struct chat));
	if (c == NULL) {
		perror("malloc failed");
		return NULL;
	}
	if (chat_id == 0) {
		return build_chat(all_chats, u);
	}
	int i = get_map(all_chats, chat_id, c, sizeof(uint16_t));
	if (i == -1)
		return NULL;
	else
		return c;
}
size_t get_ntickets(struct chat *c)
{
	if (c == NULL) return 0;
	else return c->tickets->size;
}
void free_chat(void *c){
	free_array(((struct chat *)c)->tickets, NULL);
	free(c);
}