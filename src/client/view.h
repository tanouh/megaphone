#ifndef MEGAPHONE_VIEW_H
#define MEGAPHONE_VIEW_H

#include <arpa/inet.h>
#include "../reqcode.h"
#include "../array.h"

char *ask_pseudo();

void welcome(int already_registered);

void show_id(uint16_t id);

enum reqcode choose_action();

int choose_thread_to_post_in();

char *post_ticket(int num_thread);

char *add_file_to_thread();

int select_thread_to_unfold();

int see_previous_tickets(int num_thread);

int download_file();

int subscribe_to_thread();

void print_error(char *error);

void print_notifications(struct array *q);

#endif
