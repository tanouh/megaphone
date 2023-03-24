#include "saction.h"
#include "smesslib.h"
#include "../chat.h"
#include "../ticket.h"
#include "../array.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

//variable globale : liste des users 

void *execute_action(void *arg){
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL; 
	
	void *ret_msg;
	int size_msg = 0;

	int ret = get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	free(arg);

	int index = -1;

	if (ret == -1) {
		return fill_error(&size_msg);
	}
	switch (req) {
		case INSCRIPTION : //TODO
			break;
		case PUSH_MESS : //TODO
			index = push_mess(id,chat,datalen, data);
			if (index == -1 ){
				ret_msg = fill_error(&size_msg);
			}else{
				ret_msg = fill_message(req,index,chat,nb,&size_msg);
			} 
			break ;
		case ASK_MESS : // TODO
			break ; 
		case SUBSCRIBE : //TODO
			break ;
		case PUSH_FILE : //TODO
			break ;
		case PULL_FILE : //TODO
			break ;
		default : 
			return fill_error(&size_msg); 
	}
	return ret_msg;
}

int push_mess(uint16_t id, uint16_t chat, uint16_t datalen, void *data){
	// struct array ulist = NULL; // TODO
	struct chat *c;
	struct ticket *t;

	/*if((get_user(id,ulist)) == NULL){ //TODO : recherche dans un hashmap plutôt
		perror("User not found "); //GERER
		return -1;
	}*/
	if((c = get_chat(chat)) == NULL){ 
		perror("chat not found/couldn't be created"); //GERER
		return -1;
	}
	if( (t = build_ticket(id, c, datalen, (char *)data, 0)) == NULL) {
		perror("ticket creation failer "); //GERER 
		return -1;
	}
	if(add_tickets_to_chat(c, t) == -1) {
		perror("ticket couldn't be added");
		return -1;
	}
	return c->id;
}

