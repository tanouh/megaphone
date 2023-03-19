#ifndef MEGAPHONE_CHAT_H
#define MEGAPHONE_CHAT_H

struct chat{
	int id; 
	int nbMessages;
	/* Tableau dynamique ticket contents */
	/* Tableau dynamique user followers */
};
extern int chat_counter; 

struct chat *build_chat();
void add_tickets_to_chat(int chat_id, struct ticket *t);
struct chat *find_chat(int chat_id);
/* get n_tickets_in_chat() retourne un tableau dynamique de tickets (?) */ 
void print_chat(int chat_id, int nbMsg);

#endif