#include "test_smesslib.h"

#include "../../src/constants.h"
#include "../../src/lib.h"
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

static int test_fill_inscription(void *arg);
static int test_fill_push_message(void *arg);
static int test_fill_ask_messages(void *arg);
static int test_fill_asked_messages(void *arg);
static int test_fill_subscribe(void *arg);
static int test_fill_push_file(void *arg);
static int test_fill_pull_file(void *arg);
static int test_fill_push_file_udp(void *arg);
static int test_fill_error(void *arg);
static int test_fill_notification(void *arg);
static int test_get_message(void *arg, enum reqcode *req, uint16_t *id,
			    uint16_t *chat, uint16_t *nb, uint8_t *datalen,
			    void **data);

void *test_smesslib(void *data)
{
	char buf[512];
	memset(buf, 0, 512);
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
	int size = 0;
	char *msg = fill_inscription(FIELD, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}
static int test_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_push_message(FIELD, FIELD, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return 0;
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
	char *msg = fill_subscribe(FIELD, FIELD, FIELD, MULT, &size);
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
	char *msg = fill_push_file(FIELD, FIELD, &size);
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
	char *msg = fill_pull_file(FIELD, FIELD, FIELD, &size);
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
static int test_fill_error(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_error(&size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	sleep(1); // TODO: Improve sync
	return ans;
}
static int test_fill_notification(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg = fill_notification(FIELD, NAME, TEXT, TEXT_SIZE, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	sleep(1); // TODO: Improve sync
	return ans;
}

static int test_fill_asked_messages(void *arg)
{
	int client = *(int *)arg;
	int size = 0;
	char *msg =
		fill_asked_message(FIELD, NAME, NAME, TEXT_SIZE, TEXT, &size);
	send(client, msg, size, 0);
	free(msg);
	uint8_t ans;
	recv(client, &ans, sizeof(ans), 0);
	return ans;
}

static int test_get_message(void *arg, enum reqcode *req, uint16_t *id,
			    uint16_t *chat, uint16_t *nb, uint8_t *datalen,
			    void **data)
{
	int client = *(int *)arg;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	recv(client, buf, BUFSIZ, 0);
	get_message(buf, req, id, chat, nb, datalen, data);
	return 1;
}

static int test_get_fill_inscription(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0;
	char *data = NULL;
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	print_serv("Waiting for messages...\n");
	recv(client, buf, BUFSIZ, 0);
	get_inscription(buf, &req, &id, &data);
	uint8_t ret = 1;
	ret &= ASSERT(req == INSCRIPTION);
	ret &= ASSERT(id == 0);
	ret &= ASSERT(strncmp(data, NAME, NAMELEN) == 0);
	printf("%u\n", id);
	free(data);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_message(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL;
	test_get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == PUSH_MESS);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == 0);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	free(data);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_ask_messages(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL;
	test_get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == ASK_MESS);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == FIELD);
	ret &= ASSERT(datalen == 0);
	ret &= ASSERT(data == NULL);
	send(client, &ret, sizeof(ret), 0);
	free(data);
	return (int)ret;
}
static int test_get_fill_subscribe(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL;
	test_get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == SUBSCRIBE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == 0);
	ret &= ASSERT(datalen == 0);
	ret &= ASSERT(data == NULL);
	send(client, &ret, sizeof(ret), 0);
	return (int)ret;
}
static int test_get_fill_push_file(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL;
	test_get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == PUSH_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == 0);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(data);
	return (int)ret;
}
static int test_get_fill_pull_file(void *arg)
{
	int client = *(int *)arg;
	enum reqcode req = 0;
	uint16_t id = 0, chat = 0, nb = 0;
	uint8_t datalen = 0;
	char *data = NULL;
	test_get_message(arg, &req, &id, &chat, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == PULL_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(chat == FIELD);
	ret &= ASSERT(nb == FIELD);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(data);
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
	get_udp(buf, size, &req, &id, &nb, &datalen, (void **)&data);
	uint8_t ret = 1;
	ret &= ASSERT(req == PUSH_FILE);
	ret &= ASSERT(id == FIELD);
	ret &= ASSERT(nb == FIELD);
	ret &= ASSERT(datalen == TEXT_SIZE);
	ret &= ASSERT(strncmp(data, TEXT, TEXT_SIZE) == 0);
	send(client, &ret, sizeof(ret), 0);
	free(data);
	return (int)ret;
}
