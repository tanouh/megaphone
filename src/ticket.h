#ifndef MEGAPHONE_TICKET_H
#define MEGAPHONE_TICKET_H
struct ticket {
	struct user *owner;
	struct chat *feed;
	char *data;
	int isFile; /*0 false 1 true*/
};
struct ticket *build_ticket(struct user *owner, int chat_id, char *data, int isFile);
void print_ticket(struct ticket *tic);

#endif