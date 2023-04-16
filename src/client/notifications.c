#include "notifications.h"

#include "mutex.h"
#include "../msghead.h"
#include "../constants.h"
#include "cmesslib.h"

#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define RELOAD 5

static int add_notitication(int sock);
static void copy_notfds(struct pollfd **fds, nfds_t *nfds);

struct array *notfds = NULL;
struct array *notmes = NULL;

void init_notfds()
{
	notfds = make_array(sizeof(struct pollfd));
	notmes = make_array(sizeof(struct notification));
}

void free_notfds()
{
	free_array(notfds, NULL);
	notfds = NULL;
	free_array(notmes, NULL);
	notmes = NULL;
}

int add_fd_to_notfds(int fd)
{

	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		perror("add_fd_to_notifds");
		return -1;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("add_fd_to_notifds");
		return -1;
	}

	struct pollfd pfd = {.fd = fd, .events = POLLIN, .revents = 0};
	pthread_mutex_lock(&mutex[M_NOTFDS]);
	push_back(notfds, &pfd);
	pthread_mutex_unlock(&mutex[M_NOTFDS]);
	return 0;
}

size_t count_notifications()
{
	size_t val = 0;
	pthread_mutex_lock(&mutex[M_NOTFDS]);
	val = notfds->size;
	pthread_mutex_unlock(&mutex[M_NOTFDS]);
	return val;
}

void *listen_notifications()
{
	struct pollfd *fds = NULL;
	nfds_t nfds = 0;
	while (1) {
		copy_notfds(&fds, &nfds);
		if (fds == NULL || nfds == 0) {
			perror("listen_notifications");
			sleep(RELOAD);
			continue;
		}
		int size = poll(fds, nfds, -1);
		if (size == -1) {
			perror("listen_notifications");
			sleep(RELOAD);
			continue;
		}
		for (nfds_t i = 0; i < nfds && size != 0; i++) {
			switch (fds[i].revents) {
			case 0:
				break; // ignore fds
			case POLLIN:
				add_notitication(fds[i].fd);
				break;
			default:
				perror("listen_notification");
				break;
			}
		}
		free(fds);
		fds = NULL;
		sleep(RELOAD);
	}
}

static void copy_notfds(struct pollfd **fds, nfds_t *nfds)
{
	*fds = malloc(sizeof(struct pollfd));
	if (*fds == NULL) {
		perror("copy_notfds");
		*nfds = 0;
		return;
	}
	pthread_mutex_lock(&mutex[M_NOTFDS]);
	memcpy(*fds, notfds->buf, notfds->size * sizeof(struct pollfd));
	*nfds = notfds->size;
	pthread_mutex_unlock(&mutex[M_NOTFDS]);
}

static int add_notitication(int sock) {
	struct msghead h;
	struct notification n;
	char buf[NOTIFICATION_BUFSIZ];
	memset(buf, 0, NOTIFICATION_BUFSIZ);
	memset(&n, 0, sizeof(n));
	memset(&h, 0, sizeof(h));
	int size = recv(sock, buf, NOTIFICATION_CONTENT, 0);
	if (size != NOTIFICATION_BUFSIZ) {
		fprintf(stderr, "add_notification : unvalid size");
		return -1;
	}
	get_notification_message(buf, &h, &n.author, &n.content);
	pthread_mutex_lock(&mutex[M_NOTMESS]);
	push_back(notmes, &n);
	pthread_mutex_unlock(&mutex[M_NOTMESS]);
	return 0;
}

