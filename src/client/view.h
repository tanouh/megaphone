#ifndef MEGAPHONE_VIEW_H
#define MEGAPHONE_VIEW_H

#include <arpa/inet.h>

char *ask_pseudo();

void welcome(int already_registered);

void show_id(uint16_t id);

char *choose_action();

char *choose_thread_to_post_in();

char *post_ticket(char *thread);

char *add_file();

char *select_thread_to_unfold();

char *see_previous_tickets(char *thread);

char *download_file();

char *subscribe_to_thread();

void print_error(char *error);

#endif