#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <readline/readline.h>
#include <readline/history.h>


#define SERVER_PORT 4242
#define SERVER_ADDR "fdc7:9dd5:2c66:be86:4849:43ff:fe49:79be/64"
#define SIZE_MSG 100

int registering(int socket){

        printf("Bienvenue sur le serveur *Nom Provisoire*\n");
        printf("Veuillez saisir un pseudo de 10 caractères\n");

        char *buff_send;
        memset(&buff_send,0,SIZE_MSG+1);
        fgets(buff_send,SIZE_MSG+1,STDIN_FILENO);

        int envoye = send(socket, buff_send, SIZE_MSG * sizeof(char),0);
        if (envoye < 0){
                perror("Erreur d'envoi");
                return 1;
        }

        char buff_rcv[SIZE_MSG];
        memset(&buff_rcv,0,SIZE_MSG+1);

        int recu = recv(socket, buff_rcv, SIZE_MSG * sizeof(char),0);
        if(recu < 0){
                perror("Erreur de réception");
                return 1;
        }

        buff_rcv[recu] = '\0';
        
        return 0; 
}

int main(){

        /* ****Création de la socket ****   */
        int sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == -1){
                perror("Création de socket");
                exit(1);
        }

        /* ****Création de l'adresse destinataire (serveur) **** */
        struct sockaddr_in address_sock;
        memset(&address_sock,0,sizeof(address_sock));
        address_sock.sin_family = AF_INET;
        address_sock.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET,SERVER_ADDR,&address_sock.sin_addr);


        /* ****demande de connexion au serveur **** */
        int r = connect(sock, (struct sockaddr *)&address_sock, sizeof(address_sock));

        if(r == -1){
                perror("Echec de connexion");
                exit(2);
        }

        /*if (premiere connexion)*/ registering(sock);
        //else login(sock);
        
        close(sock);
        return 0;
}