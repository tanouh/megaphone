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

static int test_get_fill_inscription(void *arg);
static int test_get_fill_push_message(void *arg);
static int test_get_fill_ask_messages(void *arg);
static int test_get_fill_asked_messages(void *arg);
static int test_get_fill_subscribe(void *arg);
static int test_get_fill_push_file(void *arg);
static int test_get_fill_pull_file(void *arg);
static int test_get_fill_push_file_udp(void *arg);
static int test_get_fill_error(void *arg);
static int test_get_fill_notification(void *arg);
static int test_get_message(void *arg, enum reqcode *req, uint16_t *id,
			    uint16_t *chat, uint16_t *nb);

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

	return ret;
}

static int test_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_inscription("TEST", 4, &size);
	print_client("Sending message...\n");
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	print_client("Waiting for answer...\n");
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_push_message(FIELD, FIELD, TEXT_SIZE, TEXT, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_ask_messages(FIELD, FIELD, FIELD, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_subscribe(FIELD, FIELD, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_push_file(FIELD, FIELD, TEXT_SIZE, TEXT, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_pull_file(FIELD, FIELD, FIELD, TEXT_SIZE, TEXT, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_push_file_udp(FIELD, FIELD, TEXT_SIZE, TEXT, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_get_message(void *arg, enum reqcode *req, uint16_t *id,
			    uint16_t *chat, uint16_t *nb)
{
	int client = *(int *)arg;
	char buf[HEADER_SERVER];
	memset(buf, 0, HEADER_SERVER);
	recv(client, buf, HEADER_SERVER, 0);
	get_message(buf, req, id, chat, nb);
	return 1;
}

static int test_get_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	uint8_t ret = 1;
	ret &= ASSERT(req == INSCRIPTION);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == 0);
	ret &= ASSERT(nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	u_int8_t ret = 1;
	ret &= ASSERT(req == PUSH_MESS);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	u_int8_t ret = 1;
	ret &= ASSERT(req == ASK_MESS);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == FIELD);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_asked_messages(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	uint16_t chat;
	uint8_t datalen;
	char *owner, *origin, *data;
	get_asked_messages(buf, &chat, (void **)&origin, (void **)&owner,
			   &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(strncmp(origin, NAME, NAMELEN) == 0);
	ret &= ASSERT(strncmp(owner, NAME, NAMELEN) == 0);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, NAMELEN) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(origin);
	free(owner);
	free(data);
	return (int)ret;
}
static int test_get_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	enum reqcode req;
	uint16_t chat, nb, id;
	char *addr;
	get_subscribed_message(buf, &req, &id, &chat, &nb, (void **)&addr);
	int ret = 1;
	ret &= ASSERT(req == SUBSCRIBE);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == FIELD);
	ret &= ASSERT(strncmp(addr, MULT, ADDRMULT_LEN));
	send(client, &ret, sizeof(ret), 0);
	free(addr);
	return (int)ret;
}
static int test_get_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	u_int8_t ret = 1;
	ret &= ASSERT(req == PUSH_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	u_int8_t ret = 1;
	ret &= ASSERT(req == PULL_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == FIELD);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, nb = 0;
	uint16_t datalen = 0;
	char *data = NULL;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	int size = recv(client, buf, BUFSIZ, 0);
	get_udp_message(buf, size, &req, &id, &nb, &datalen, (void **)&data);
	int ret = 1;
	ret &= ASSERT(req == PUSH_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(nb == FIELD);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(data);
	return ret;
}
static int test_get_fill_error(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	test_get_message(arg, &req, &id, &chat, &nb);
	u_int8_t ret = 1;
	ret &= ASSERT(req == ERROR);
	ret &= ASSERT(id == 0);
	ret &= ASSERT(chat == 0);
	ret &= ASSERT(nb == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_notification(void *arg)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	enum reqcode req;
	uint16_t chat, id;
	char *owner, *data;
	get_notification_message(buf, &req, &id, &chat, (void **)&owner,
				 (void **)&data);
	int ret = 1;
	ret &= ASSERT(req == SUBSCRIBE);
	ret &= ASSERT(id == 0);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(strncmp(owner, NAME, NAMELEN) == 0);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(owner);
	free(data);
	return (int)ret;
}
