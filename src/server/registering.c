#include "../constants.h"
#include "../reqcode.h"
#include "registering.h"
#include "smesslib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SIZE_MSG 256

uint16_t new_id(uint16_t *next_id)
{
	uint16_t ret = *next_id;
	(*next_id)++;
	return ret;
}

int compare_identifiers(void *key1, void *key2)
{
	return strcmp((char *)key1, (char *)key2) == 0;
}

void free_identifier(void *id)
{
	free(id);
}

void free_nickname(void *nickname)
{
	free(nickname);
}

int accept_registering(int sockclient, struct map *identifiers,
		       uint16_t *next_id)
{
	char buff_rcv[SIZE_MSG];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(sockclient, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de reception");
		return -1;
	}

	enum reqcode req = 0;
	uint16_t id;
	char *nickname;

	if(get_inscription(buff_rcv, &req, &id, &nickname) == -1){
		perror("get_incripition");
		return -1;
	}
	if (reqtoi(req) != 1) {
		perror("Erreur de requete");
		return -1;
	}

	id = new_id(next_id);

	if (!put_map(identifiers, &id, &nickname, NULL, sizeof(uint16_t),
		     sizeof(char *))) {
		perror("Erreur de stockage de l'identifiant");
		return -1;
	}

	return 0;
}