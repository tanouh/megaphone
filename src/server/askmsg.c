#include "askmsg.h"
#include "chat.h"
#include "smesslib.h"
#include "../msghead.h"
#include "../constants.h"
#include "../ticket.h"
#include "../map.h"


#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

pthread_mutex_t maskall = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t maskmsg = PTHREAD_MUTEX_INITIALIZER;


int get_last_n_messages(struct map *identifiers, struct msghead *h, struct chat *c,
	void *buf, int sizebuf);

int ask_mess (struct map *identifiers, struct msghead *h, 
	void *buf, int sizebuf)
{
	int res = 0;
	struct chat *c;

	if (h->chat != 0){
		pthread_mutex_lock(&maskmsg);
		if(get_map(identifiers, &h->chat, c, sizeof(h->chat)) == -1){
			perror("Couldn't get the chat");
			return -1;
		}
		pthread_mutex_unlock(&maskmsg); 
		if (h->nb == 0){
			h->nb = c->nbMessages;
		}
		res = get_last_n_messages(identifiers, h, c, buf, sizebuf);
	}else{
		uint16_t id;
		map_foreach(all_chats, &id, &c){
			res = get_last_n_messages(identifiers, h, c, buf, 
				sizebuf);
		}
	}
	return res;
}
	
int get_last_n_messages(struct map *identifiers, struct msghead *h, struct chat *c,
	void *buf, int sizebuf)
{	
	int res = 0 ;

	int i = c->nbMessages - h->nb;
	if (i < 0) i = 0;
	int ret = 0 ; 

	char origin[NAMELEN];
	char owner[NAMELEN];
	struct ticket *t = NULL;
	uint16_t originid = 0;

	while (i < h->nb){
		if (sizebuf - res < 0){
			perror("Buf overflowed");
			break;
		}
		memset(origin, 0, NAMELEN);
		memset(owner,0,NAMELEN);

		// récupérer les données du ticket
		t = at(c->messages, i);
		h->datalen = t->datalen;
		originid = c->origin_user;
		
		pthread_mutex_lock(&maskall);
		if(get_map(identifiers, &originid, origin, sizeof(originid)) != 0){
			perror("get nickname failed");
			continue ; // ou break?
		}
		if(get_map(identifiers, &t->owner, owner, sizeof(t->owner)) != 0){
			perror("get nickname failed");
			continue ; // ou break?
		}
		pthread_mutex_unlock(&maskall);

		ret = fill_asked_message(*h, buf + res, sizebuf - res, 
			origin, owner, t->data);
		if (ret == -1){
			break; 
		}
		res += ret;
		i++;
	}
	return res;
}