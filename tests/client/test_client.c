#include "../../src/client/cmesslib.h"
#include "../../src/lib.h"
#include "../printlib.h"
#include "../test_constants.h"
#include "../testlib.h"
#include "test_cmesslib.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*This is the client side test file.
It will test client functions and the communication with the server.
*/
#define NB_TEST 0

static int connect_to_server();

static void *test_network();
static int do_tests(pthread_t *pts);

static void *(*tests[NB_TEST])(void *) = {};

int main(int argc, char **argv)
{
	int net = 0;
	pthread_t pts[NB_TEST + 1];
	if (argc <= 1 || strcmp(argv[1], NONET) != 0) {
		if (pthread_create(pts, NULL, (void *(*)(void *))test_network,
				   NULL) == -1) {
			perror("Thread didn't laucnhed");
			return ESYS;
		}
		net = 1;
	}

	int ntest = do_tests(pts + net);

	int ret = 1;
	for (int i = 0; i < ntest + net; i++) {
		int *tmp;
		if (pthread_join(pts[i], (void *)&tmp) != 0)
			perror("thread join");
		if (tmp != NULL) {
			ret &= *tmp;
			free(tmp);
		} else {
			ret = 0;
		}
	}
	char buf[512];
	sprintf(buf, "End of client (%d)\n", !ret);
	print_client(buf);
	return !ret;
}

static void *test_network()
{
	int sock = connect_to_server();
	if (sock < 0) {
		print_client("Can't connect to server !\n");
		return malloc_return(1);
	}
	print_client("connected\n");
	int ret = 1;
	ret = test_cmesslib(sock);
	close(sock);
	return (ret) ? malloc_return(ret) : NULL;
}

static int connect_to_server()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_SERVER_PORT);
	if (inet_pton(AF_INET, LOCALHOST4, &addr.sin_addr) != 1)
		return -1;
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		return -1;
	return sock;
}

static int do_tests(pthread_t *pts)
{
	int ntest = NB_TEST;
	for (int i = 0; i < ntest; i++) {
		if (pthread_create(pts + i, NULL, tests[i], NULL) == -1) {
			perror("test error");
			ntest--;
			i--;
		}
	}
	return ntest;
}
