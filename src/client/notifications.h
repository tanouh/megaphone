#ifndef MEGAPHONE_NOTIFICATIONS_H
#define MEGAPHONE_NOTIFICATIONS_H
#include "../array.h"
#include <stddef.h>
#include <limits.h>
#include <poll.h>
#include <arpa/inet.h>
#include "../constants.h"

struct notification {
	char author[NAMELEN + 1];
	uint16_t chat;
	char content[NOTIFICATION_CONTENT + 1];
};

extern struct array *notfds;

void init_notfds();

void free_notfds();

int add_fd_to_notfds(int fd);

size_t count_notifications();

void *listen_notifications();

#endif
