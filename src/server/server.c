#include "../constants.h"
#include "../lib.h"
#include "../map.h"
#include "smesslib.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "smesslib.h"
#include "saction.h"
#include "../constants.h"
#include "../lib.h"
#include "../chat.h"
#include "../array.h"


#define NBCLIENTS 10
#define SIZE_MSG 256

int server;
uint16_t next_id;
struct map *identifiers;
int c_connected;
pthread_t threads[NBCLIENTSMAX];
pthread_mutex_t mserv = PTHREAD_MUTEX_INITIALIZER;

void decrease_c_connected(){
	/* A la fin de la connexion on décrémente le nombre de clients connectés*/
	pthread_mutex_lock(&mserv);
	c_connected--;
	pthread_mutex_unlock(&mserv);
}

int get_server_port(int argc, char *argv[])
{
	if (argc != 2)
		return PORT;
	else
		return atoi(argv[1]);
}

int initialise_data()
{
	all_chats = make_array(sizeof(struct chat));
	c_connected = 0;
	return 0;
}

int create_server(int port)
{
	struct sockaddr_in6 address_sock;
	memset(&address_sock, 0, sizeof(address_sock));
	address_sock.sin6_family = AF_INET6;
	address_sock.sin6_port = htons(port);
	address_sock.sin6_addr = in6addr_any;

	server = socket(PF_INET6, SOCK_STREAM, 0);
	if (server < 0) {
		perror("fail in socket creation");
		return -1;
	}
	int optval = 0;
	if (setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, &optval,
		       sizeof(optval)) < 0)
		perror("connexion with IPV4 impossible");

	optval = 1;
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval,
		       sizeof(optval)) < 0)
		perror("port reuse impossible");
	if (bind(server, (struct sockaddr *)&address_sock,
		 sizeof(address_sock)) < 0) {
		perror("binding failed");
		return -1;
	}
	if (listen(server, 0) < 0) {
		perror("listen error");
		return -1;
	}
	return 0;
}

uint16_t new_id()
{
	uint16_t ret = next_id;
	next_id++;
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

int accept_inscription(void *sockclient)
{
	char buff_rcv[SIZE_MSG];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(*(int *)sockclient, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de reception");
		return -1;
	}

	enum reqcode *req;
	uint16_t id;
	char *nickname;
	get_inscription(buff_rcv, req, &id, &nickname);
	if (reqtoi(*req) != 1) {
		perror("Erreur de requete");
		return -1;
	}

	id = new_id();

	if (!put_map(identifiers, &id, &nickname, NULL, sizeof(uint16_t),
		     sizeof(char *))) {
		perror("Erreur de stockage de l'identifiant");
		return -1;
	}

	return 0;
}

void *init(void *sockclient)
{
	print_s("Connexion établie\n");

	int sock = *(int *)sockclient;
	free(sockclient);
	char msg_rcv[SBUF];
	memset(msg_rcv, 0 , sizeof(msg_rcv));

	int rcv = recv(sock, msg_rcv, SBUF,0);
	if(rcv < 0){
		perror("Message not received");
	}if(rcv == 0){
		perror("send null");
	}else{
		execute_action((void *)msg_rcv);
	}
	decrease_c_connected();
	close(sock);
	return NULL;
}

int connect_to_client () {
	/* le serveur accepte une connexion
	et crée la socket de communication avec le client */
	int *sockclient = malloc(sizeof(int));
	if(sockclient == NULL){
		perror("malloc failed");
		close(server);
		return 1;
	}
	struct sockaddr_in6 adrclient;
	memset(&adrclient, 0, sizeof(adrclient));
	socklen_t size = sizeof(adrclient);
	*sockclient = accept(server, (struct sockaddr *)&adrclient, &size);

	if (*sockclient < 0) {
		return -1;
	}
	if (pthread_create(threads+c_connected, NULL, init, sockclient) == -1){
		perror("Thread creation failed");
		return -1;
	}
	return 0;
}

int serve(int port){
	if(create_server(port)==-1)
		return 1;
	print_s("Ouverture du serveur \nEN ATTENTE DE CONNEXION...\n");

	if (initialise_data() != 0 ) return 1;
	/*
	Suggestion : créer un thread qui écoute constamment le terminal 
	responsable de la terminaison du serveur 
	*/

	while(c_connected < NBCLIENTSMAX){
		if(connect_to_client() == -1) break;
		c_connected ++;
	}

	next_id = 1;
	identifiers = make_map(compare_identifiers, default_hash);

	int ret = 1;
	int *tmp;
	for (int i = 0; i < c_connected; i++) {
		if (pthread_join(threads[i], (void **)&tmp) != 0)
			perror("thread join");
		if (tmp != NULL) {
			ret |= *tmp;
			free(tmp);
		} else {
			ret = 0;
		}
	}
	
	char buf[SBUF];
	sprintf(buf, "Fin de session (%d)\n", !ret);
	free_map(identifiers, free_identifier, free_nickname);
	print_s(buf);
	close(server);
	return !ret;
}

int main(int argc, char *argv[])
{
	int port = get_server_port(argc, argv);
	
	return serve(port);
}
