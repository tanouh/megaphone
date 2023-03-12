#ifndef MEGAPHONE_VIEW_H
#define MEGAPHONE_VIEW_H

#include <arpa/inet.h>

char *ask_pseudo();

void show_id(uint16_t id);

char *choose_action();

void print_error(char *error);

#endif