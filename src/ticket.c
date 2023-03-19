#include "ticket.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

struct ticket *build_ticket(struct user *owner, int chat_id, char *data, int isFile){
	struct ticket *t = malloc(sizeof(struct ticket));
	if(t==NULL){
		perror("malloc ticket failed");
		return NULL;
	}
	t->owner = *owner;
	t->feed = find_chat(chat_id);
	t->data = data;
	t->isFile = isFile;
	return t;
}

void print_ticket(struct ticket *t){
	printf("Auteur : %s\nContenu : %s", *t->owner->name, *t->data);
	/*
	Auteur : 
	Contenu : */
}