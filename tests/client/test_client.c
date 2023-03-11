#include "../../src/client/cmesslib.h"
#include "../test_constants.h"
#include "../testlib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "../printlib.h"

/*This is the client side test file.
It will test client functions and the communication with the server.
*/

int connect_to_server()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, LOCALHOST4, &addr.sin_addr) != 1)
		return -1;
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		return -1;
	return sock;
}

int main()
{
	int sock = connect_to_server();
	if (sock < 0) {
		print_client("Can't connect to server !\n", 1);
		return 1;
	}
	send(sock, "HELLO", 6, 0);
	print_client("connected\n", 1);
	close(sock);
	return 0;
}
