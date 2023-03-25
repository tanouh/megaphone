#include "../constants.h"
#include "../reqcode.h"
#include "registering.h"
#include "smesslib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

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

	char *nickname = malloc(NAMELEN + 1);
	if (nickname == NULL){
		perror("malloc");
		return -1;
	}
	struct msghead h;
	memset(nickname, 0, NAMELEN);
	memset(&h, 0, sizeof(h));

	if(get_inscription(buff_rcv, &h, nickname) == -1){
		perror("get_incription");
		free(nickname);
		return -1;
	}
	if (h.req != INSCRIPTION) {
		perror("Erreur de requete");
		free(nickname);
		return -1;
	}

	h.id = new_id(next_id);

	if (!put_map(identifiers, &(h.id), &nickname, NULL, sizeof(uint16_t),
		     sizeof(char *))) {
		perror("Erreur de stockage de l'identifiant");
		free(nickname);
		return -1;
	}

	return 0;
}
