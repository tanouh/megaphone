#include "test_cmesslib.h"

#include "../../src/client/cmesslib.h"
#include "../../src/constants.h"
#include "../printlib.h"
#include "../test_constants.h"
#include "../testlib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int test_get_fill_inscription(void *arg);
int test_get_fill_push_message(void *arg);
int test_get_fill_ask_messages(void *arg);
int test_get_fill_asked_messages(void *arg);
int test_get_fill_subscribe(void *arg);
int test_get_fill_push_file(void *arg);
int test_get_fill_pull_file(void *arg);
int test_get_fill_push_file_udp(void *arg);
int test_get_fill_error(void *arg);
int test_get_fill_notification(void *arg);
int test_get_message(void *arg, struct msghead *h);

static int test_fill_inscription(void *arg);
static int test_fill_push_message(void *arg);
static int test_fill_ask_messages(void *arg);
static int test_fill_subscribe(void *arg);
static int test_fill_push_file(void *arg);
static int test_fill_pull_file(void *arg);
static int test_fill_push_file_udp(void *arg);

int test_cmesslib(int sock)
{
	int ret = 1;
	// CLIENT
	ret &= test_carg(test_fill_inscription, (void *)&sock,
			 "test_fill_inscription", print_client);
	ret &= test_carg(test_fill_push_message, (void *)&sock,
			 "test_fill_push_message", print_client);
	ret &= test_carg(test_fill_ask_messages, (void *)&sock,
			 "test_fill_ask_messages", print_client);
	ret &= test_carg(test_fill_subscribe, (void *)&sock,
			 "test_fill_subscribe", print_client);
	ret &= test_carg(test_fill_push_file, (void *)&sock,
			 "test_fill_push_file", print_client);
	ret &= test_carg(test_fill_pull_file, (void *)&sock,
			 "test_fill_pull_file", print_client);
	ret &= test_carg(test_fill_push_file_udp, (void *)&sock,
			 "test_fill_push_file_udp", print_client);
	ret &= test_carg(test_get_fill_inscription, (void *)&sock,
			 "test_get_fill_inscription", print_client);
	ret &= test_carg(test_get_fill_push_message, (void *)&sock,
			 "test_get_fill_push_message", print_client);
	ret &= test_carg(test_get_fill_ask_messages, (void *)&sock,
			 "test_get_fill_ask_messages", print_client);
	ret &= test_carg(test_get_fill_asked_messages, (void *)&sock,
			 "test_get_fill_asked_messages", print_client);
	ret &= test_carg(test_get_fill_push_file, (void *)&sock,
			 "test_get_fill_push_file", print_client);
	ret &= test_carg(test_get_fill_pull_file, (void *)&sock,
			 "test_get_fill_pull_file", print_client);
	ret &= test_carg(test_get_fill_push_file_udp, (void *)&sock,
			 "test_get_fill_push_file_udp", print_client);
	ret &= test_carg(test_get_fill_error, (void *)&sock,
			 "test_get_fill_error", print_client);
	ret &= test_carg(test_get_fill_notification, (void *)&sock,
			 "test_get_fill_notification", print_client);
	ret &= test_carg(test_get_fill_subscribe, (void *)&sock,
			 "test_get_fill_subscribe", print_client);

	return ret;
}

static int test_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(INSCRIPTION, 0, 0, 0, 0);
	char msg[BUFSIZ];
	int size = fill_inscription(h, msg, BUFSIZ, "TEST", 4);
	print_client("Sending message...\n");
	send(client, msg, size, 0);
	uint8_t ans;
	print_client("Waiting for answer...\n");
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(PUSH_MESS, FIELD, FIELD, 0, TEXT_SIZE);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	char msg[BUFSIZ];
	struct msghead h = fill_msghead(ASK_MESS, FIELD, FIELD, FIELD, 0);
	int size = fill_message(h, msg, BUFSIZ, NULL);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(SUBSCRIBE, FIELD, FIELD, 0, 0);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ, NULL);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(PUSH_FILE, FIELD, FIELD, 0, TEXT_SIZE);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	char msg[BUFSIZ];
	struct msghead h =
		fill_msghead(PULL_FILE, FIELD, FIELD, FIELD, TEXT_SIZE);
	int size = fill_message(h, msg, BUFSIZ, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(PUSH_FILE, FIELD, 0, FIELD, TEXT_SIZE);
	char msg[BUFSIZ];
	memset(msg, 0, BUFSIZ);
	int size = fill_udp(h, msg, BUFSIZ, TEXT);
	if (size == -1)
		return 0;
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

int test_get_message(void *arg, struct msghead *h)
{
	int client = *(int *)arg;
	char buf[HEADER_SERVER];
	memset(buf, 0, HEADER_SERVER);
	recv(client, buf, HEADER_SERVER, 0);
	get_message(buf, h);
	return 1;
}

static int test_get_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == INSCRIPTION);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == 0);
	ret &= ASSERT(h.nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	u_int8_t ret = 1;
	ret &= ASSERT(h.req == PUSH_MESS);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	u_int8_t ret = 1;
	ret &= ASSERT(h.req == ASK_MESS);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_asked_messages(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	char owner[NAMELEN + 1], origin[NAMELEN + 1], data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	owner[NAMELEN] = 0;
	origin[NAMELEN] = 0;
	get_asked_messages(buf, &h, origin, owner, data, BUFSIZ);
	uint8_t ret = 1;
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(strncmp(origin, NAME, NAMELEN) == 0);
	ret &= ASSERT(strncmp(owner, NAME, NAMELEN) == 0);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, NAMELEN) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	char addr[ADDRMULT_LEN + 1];
	addr[ADDRMULT_LEN] = 0;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	get_subscribed_message(buf, &h, addr);
	int ret = 1;
	ret &= ASSERT(h.req == SUBSCRIBE);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	ret &= ASSERT(strncmp(addr, MULT, ADDRMULT_LEN));
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	u_int8_t ret = 1;
	ret &= ASSERT(h.req == PUSH_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == 0);
	printf("h.nb = %ud\n", h.nb);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	u_int8_t ret = 1;
	ret &= ASSERT(h.req == PULL_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	char data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	int size = recv(client, buf, BUFSIZ, 0);
	get_udp_message(buf, size, &h, data, BUFSIZ);
	int ret = 1;
	ret &= ASSERT(h.req == PUSH_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return ret;
}
static int test_get_fill_error(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h);
	u_int8_t ret = 1;
	ret &= ASSERT(h.req == ERROR);
	ret &= ASSERT(h.id == 0);
	ret &= ASSERT(h.chat == 0);
	ret &= ASSERT(h.nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_notification(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	char owner[NAMELEN + 1], data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	memset(owner, 0, NAMELEN);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	get_notification_message(buf, &h, &owner, &data);
	int ret = 1;
	ret &= ASSERT(h.req == SUBSCRIBE);
	ret &= ASSERT(h.id == 0);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(strncmp(owner, NAME, NAMELEN) == 0);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
