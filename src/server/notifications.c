#include "notifications.h"
#include "chat.h"
#include "../msghead.h"
#include "../constants.h"
#include "../map.h"
#include "server_constants.h"
#include "mutex.h"
#include "server_constants.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <net/if.h>
#include <pthread.h>

static int notify_chat(struct chat chat);

void *notify(void *unused) {
	pthread_mutex_lock(&mutex[M_ALL_CHATS]);
	struct map *chats = copy_map(all_chats,NULL, NULL);
	pthread_mutex_unlock(&mutex[M_ALL_CHATS]);
	struct chat c;
	map_foreach_data(chats, &c) {
		if (c.addrmult != 0 && c.followers > 0)
			notify_chat(c);
	}
	return NULL;
}

static int config_sock(char *addrmult) {
	int sock = socket(PF_INET6, SOCK_DGRAM, 0);
	int no = 0;
	int yes = 1;
	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, addrmult, &addr.sin6_addr);
	addr.sin6_port = htons(mult_port);
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no)) < 0)
		return -1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		return -1;
	int ifindex = if_nametoindex(mult_index_name);
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifindex, sizeof(ifindex)))
		return -1;
	return sock;

}


static int notify_chat(struct chat chat) {
	int sock = config_sock(chat.addrmult);

}
