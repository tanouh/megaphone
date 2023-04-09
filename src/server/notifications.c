#include "notifications.h"

#include "../array.h"
#include "../constants.h"
#include "../map.h"
#include "../msghead.h"
#include "../ticket.h"
#include "chat.h"
#include "mutex.h"
#include "server_constants.h"
#include "smesslib.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NOTBUF 34
#define SLEEPTIME 600 // 10 minutes

static int notify_chat(struct chat chat, struct map *ids);
static void update_all_chats(struct map *chats);
static void trim_content(struct ticket *t, char *buf);
static int config_sock();

void *notify(void *ids)
{
	while(1) {
		sleep(SLEEPTIME);
		launch_notifications(ids);
	}
	return NULL;
}

void launch_notifications(void *ids) {
	pthread_mutex_lock(&mutex[M_IDS]);
	struct map *map_ids = copy_map(ids, NULL, NULL);
	pthread_mutex_unlock(&mutex[M_IDS]);
	pthread_mutex_lock(&mutex[M_ALL_CHATS]);
	struct map *chats = copy_map(all_chats, NULL, NULL);
	pthread_mutex_unlock(&mutex[M_ALL_CHATS]);
	struct chat c;
	map_foreach_data (chats, &c) {
		if (c.addrmult != 0 && c.followers > 0)
			notify_chat(c, map_ids);
	}
	pthread_mutex_lock(&mutex[M_ALL_CHATS]);
	update_all_chats(chats);
	pthread_mutex_unlock(&mutex[M_ALL_CHATS]);
	free_map(chats, NULL, (void (*)(void *))destruct_chat);
	free_map(map_ids, NULL, free);
}

static struct sockaddr_in6 set_sockaddr(char *addrmult)
{
	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, addrmult, &addr.sin6_addr);
	addr.sin6_port = htons(mult_port);
	return addr;
}
static int config_sock()
{
	int sock = socket(PF_INET6, SOCK_DGRAM, 0);
	int no = 0;
	int yes = 1;
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no)) < 0)
		return -1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		return -1;
	int ifindex = if_nametoindex(mult_index_name);
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifindex,
		       sizeof(ifindex)))
		return -1;
	return sock;
}

static void update_all_chats(struct map *chats)
{
	uint16_t key;
	struct chat c;
	struct chat tmp;
	map_foreach (all_chats, &key, &c) {
		if (c.addrmult != 0 && c.followers > 0 &&
		    !get_map(chats, &key, &tmp, sizeof(c))) {
			c.last_message_notified = tmp.messages->size;
			put_map(all_chats, &key, &c, NULL, sizeof(key),
				sizeof(c));
		}
	}
}

static int notify_chat(struct chat chat, struct map *ids)
{
	struct sockaddr_in6 addr = set_sockaddr(chat.addrmult);
	int sock = config_sock();
	struct array *tickets =
		sub_array(chat.messages, chat.last_message_notified,
			  chat.messages->size, NULL);
	char content[NOTIFICATION_CONTENT];
	char msg[NOTBUF];
	char owner[NAMELEN];
	struct msghead h = fill_msghead(SUBSCRIBE, 0, 0, 0, 0);
	for (size_t i = 0; i < tickets->size; i++) {
		struct ticket *t = at(tickets, i);
		trim_content(t, content);
		get_map(ids, &t->owner, owner, sizeof(t->owner));
		int size = fill_notification(h, msg, NOTBUF, owner, content);
		if (sendto(sock, msg, size, 0, (struct sockaddr *)&addr,
			   sizeof(addr)) < 0)
			perror("sento mult");
	}
	free_array(tickets, NULL);
	return 0;
}

static void trim_content(struct ticket *t, char *buf)
{
	if (t->datalen > 20) {
		memset(buf, '.', NOTIFICATION_CONTENT);
		memcpy(buf, t->data, NOTIFICATION_CONTENT - 3);
	} else {
		memcpy(buf, t->data, t->datalen);
	}
}
