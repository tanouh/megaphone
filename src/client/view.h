#ifndef MEGAPHONE_VIEW_H
#define MEGAPHONE_VIEW_H

#include <arpa/inet.h>
#include "../reqcode.h"

char *ask_pseudo();

void welcome(int already_registered);

void show_id(uint16_t id);

enum reqcode choose_action();

int choose_chat_to_post_in();

char *post_ticket(int nchat);

char *add_file_to_chat();

int select_chat_to_unfold();

int see_previous_tickets(int nchat);

int download_file();

int subscribe_to_chat();

void print_error(char *error);

#endif