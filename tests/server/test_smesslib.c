#include "test_smesslib.h"

#include "../../src/constants.h"
#include "../../src/reqcode.h"
#include "../../src/server/smesslib.h"
#include "../testlib.h"
#include "../printlib.h"
#include "../test_constants.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int client = 0;

int test_fill_inscription()
{
	char *msg = fill_inscription(1);
	send(client, msg, MIN_HEADER + NAMELEN, 0);
	return 0;
}
int test_fill_push_message() {
	return 0;
}
int test_fill_ask_messages() {
	return 0;
}
int test_fill_subscribe() {
	return 0;
}
int test_fill_push_file() {return 0;}
int test_fill_pull_file() {return 0;}
int test_fill_udp() {return 0;}
int test_fill_push_file_udp() {return 0;}
int test_fill_error() {return 0;}
int test_fill_notification() {return 0;}
int test_get_message() {return 0;}
int test_get_udp() {return 0;}

void *test_smesslib(void *data)
{
	char msg[SBUF];
	char buf[512];
	memset(buf, 0, 512);
	client = *(int *)data;
	recv(client, buf, 512, 0);
	sprintf(msg, "Recieved: %s\n", buf);
	print_serv(msg);
	int ret = assert(strcmp("FAILED", buf) == 0, __LINE__, __FILE__);
	close(client);
	return NULL;
}
