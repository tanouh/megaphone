#include "test_smesslib.h"

#include "../../src/constants.h"
#include "../../src/lib.h"
#include "../../src/msghead.h"
#include "../../src/reqcode.h"
#include "../../src/server/smesslib.h"
#include "../printlib.h"
#include "../test_constants.h"
#include "../testlib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int test_get_fill_inscription(void *arg);
static int test_get_fill_push_message(void *arg);
static int test_get_fill_ask_messages(void *arg);
static int test_get_fill_subscribe(void *arg);
static int test_get_fill_push_file(void *arg);
static int test_get_fill_pull_file(void *arg);
static int test_get_fill_push_file_udp(void *arg);

int test_fill_inscription(void *arg);
int test_fill_push_message(void *arg);
int test_fill_ask_messages(void *arg);
int test_fill_asked_messages(void *arg);
int test_fill_subscribe(void *arg);
int test_fill_push_file(void *arg);
int test_fill_pull_file(void *arg);
int test_fill_push_file_udp(void *arg);
int test_fill_error(void *arg);
int test_fill_notification(void *arg);
int test_get_message(void *arg, struct msghead *h, void *data, int bufsize);

void *test_smesslib(void *data)
{
	int client = *(int *)data;
	int ret = 1;
	// CLIENT
	ret &= test_carg(test_get_fill_inscription, (void *)&client,
			 "test_get_fill_inscription", print_serv);
	ret &= test_carg(test_get_fill_push_message, (void *)&client,
			 "test_get_fill_push_message", print_serv);
	ret &= test_carg(test_get_fill_ask_messages, (void *)&client,
			 "test_get_fill_ask_messages", print_serv);
	ret &= test_carg(test_get_fill_subscribe, (void *)&client,
			 "test_get_fill_subscribe", print_serv);
	ret &= test_carg(test_get_fill_push_file, (void *)&client,
			 "test_get_fill_push_file", print_serv);
	ret &= test_carg(test_get_fill_pull_file, (void *)&client,
			 "test_get_fill_pull_file", print_serv);
	ret &= test_carg(test_get_fill_push_file_udp, (void *)&client,
			 "test_get_fill_push_file_udp", print_serv);

	// SERVER
	ret &= test_carg(test_fill_inscription, (void *)&client,
			 "test_fill_inscription", print_serv);
	ret &= test_carg(test_fill_push_message, (void *)&client,
			 "test_fill_push_message", print_serv);
	ret &= test_carg(test_fill_ask_messages, (void *)&client,
			 "test_fill_ask_messages", print_serv);
	ret &= test_carg(test_fill_asked_messages, (void *)&client,
			 "test_fill_asked_messages", print_serv);
	ret &= test_carg(test_fill_push_file, (void *)&client,
			 "test_fill_push_file", print_serv);
	ret &= test_carg(test_fill_pull_file, (void *)&client,
			 "test_fill_pull_file", print_serv);
	ret &= test_carg(test_fill_push_file_udp, (void *)&client,
			 "test_fill_push_file_udp", print_serv);
	ret &= test_carg(test_fill_error, (void *)&client, "test_fill_error",
			 print_serv);
	ret &= test_carg(test_fill_notification, (void *)&client,
			 "test_fill_notification", print_serv);
	ret &= test_carg(test_fill_subscribe, (void *)&client,
			 "test_fill_subscribe", print_serv);
	close(client);
	return (ret) ? malloc_return(ret) : NULL;
}

static int test_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(INSCRIPTION, FIELD, 0, 0, 0);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(PUSH_MESS, FIELD, FIELD, 0, 0);
	char buf[BUFSIZ];
	int size = fill_message(h, buf, BUFSIZ);
	send(client, buf, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return 0;
}
static int test_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(ASK_MESS, FIELD, FIELD, FIELD, 0);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	struct msghead h =
		fill_msghead(SUBSCRIBE, FIELD, FIELD, FIELD, ADDRMULT_LEN);
	char msg[BUFSIZ];
	int size = fill_subscribe(h, msg, BUFSIZ, MULT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(PUSH_FILE, FIELD, FIELD, 0, 0);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	char msg[BUFSIZ];
	struct msghead h = fill_msghead(PULL_FILE, FIELD, FIELD, FIELD, 0);
	int size = fill_message(h, msg, BUFSIZ);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	struct msghead h =
		fill_msghead(PUSH_FILE, FIELD, FIELD, FIELD, TEXT_SIZE);
	char msg[BUFSIZ];
	memset(msg, 0, BUFSIZ);
	int size = fill_udp(h, msg, BUFSIZ, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_error(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(ERROR, 0, 0, 0, 0);
	char msg[BUFSIZ];
	int size = fill_message(h, msg, BUFSIZ);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	sleep(1); // TODO: Improve sync
	return ans;
}
static int test_fill_notification(void *arg)
{
	int client = *(int *)arg;
	struct msghead h = fill_msghead(SUBSCRIBE, 0, FIELD, 0, TEXT_SIZE);
	char msg[BUFSIZ];
	int size = fill_notification(h, msg, BUFSIZ, NAME, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	sleep(1); // TODO: Improve sync
	return ans;
}

static int test_fill_asked_messages(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	h.chat = FIELD;
	h.datalen = TEXT_SIZE;
	char msg[BUFSIZ];
	int size = fill_asked_message(h, msg, BUFSIZ, NAME, NAME, TEXT);
	send(client, msg, size, 0);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

int test_get_message(void *arg, struct msghead *h, void *data, int bufsize)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	return get_message(buf, h, data, bufsize);
}

static int test_get_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	char data[NAMELEN + 1];
	memset(data, 0, NAMELEN);
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	print_serv("Waiting for messages...\n");
	recv(client, buf, BUFSIZ, 0);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	get_inscription(buf, &h, data);
	data[NAMELEN] = 0;
	uint8_t ret = 1;
	ret &= ASSERT(h.req == INSCRIPTION);
	ret &= ASSERT(h.id == 0);
	ret &= ASSERT(strncmp(data, NAME, NAMELEN) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	char data[TEXT_SIZE + 1];
	memset(data, 0, TEXT_SIZE + 1);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h, data, TEXT_SIZE);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == PUSH_MESS);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == 0);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h, NULL, 0);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == ASK_MESS);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	ret &= ASSERT(h.datalen == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h, NULL, 0);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == SUBSCRIBE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == 0);
	ret &= ASSERT(h.datalen == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	char data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	struct msghead h;
	memset(&h, 0, sizeof(h));
	test_get_message(arg, &h, data, TEXT_SIZE);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == PUSH_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == 0);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	char data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	test_get_message(arg, &h, data, BUFSIZ);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == PULL_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.chat == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file_udp(void *arg)
{
	int client = *(int *)arg;
	struct msghead h;
	memset(&h, 0, sizeof(h));
	char buf[BUFSIZ];
	char data[TEXT_SIZE + 1];
	memset(data, 0, TEXT_SIZE + 1);
	memset(buf, 0, BUFSIZ);
	int size = recv(client, buf, BUFSIZ, 0);
	get_udp(buf, size, &h, data, TEXT_SIZE);
	uint8_t ret = 1;
	ret &= ASSERT(h.req == PUSH_FILE);
	ret &= ASSERT(h.id == FIELD);
	ret &= ASSERT(h.nb == FIELD);
	ret &= ASSERT(h.datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
